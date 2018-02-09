#include<iostream>
using namespace std;
enum Color
{
	Red,
	Black
};
template<class K, class V>
struct RBTreeNode
{
	RBTreeNode<K, V>* _pLeft;
	RBTreeNode<K, V>* _pRight;
	RBTreeNode<K, V>* _pParent;
	K _key;
	V _value;
	Color _color;
	RBTreeNode(const K& key, const V& value)
		:_pLeft(NULL)
		, _pRight(NULL)
		, _pParent(NULL)
		, _key(key)
		, _value(value)
		, _color(Red){}
};
template<class K, class V>
class RBTree
{
public:
	typedef RBTreeNode<K, V> Node;
	typedef Node* PNode;
private:
	PNode _pRoot;
public:
	RBTree()
		:_pRoot(NULL){}
	bool Insert(const K& key, const V& value)
	{
		if (NULL == _pRoot)//���ڵ�Ϊ��ɫ
		{
			_pRoot = new Node(key, value);
			_pRoot->_color = Black;
			return true;
		}
		PNode cur = _pRoot;
		PNode pParent = NULL;
		while (cur)//�Ҳ����λ��
		{
			if (cur->_key == key)
				return false;
			else if (cur->_key > key)
			{
				pParent = cur;
				cur = cur->_pLeft;
			}
			else
			{
				pParent = cur;
				cur = cur->_pRight;
			}
		}
		//�ҵ������λ���Ժ�ʼ����
		PNode pNewNode = new Node(key, value);//�²���Ľ�����ɫĬ���Ǻ��
		if (key<pParent->_key)
		{
			pParent->_pLeft = pNewNode;
			pNewNode->_pParent = pParent;
		}
		else
		{
			pParent->_pRight = pNewNode;
			pNewNode->_pParent = pParent;
		}
		////������������Ժ����Ǿ�Ҫ���ݺ�������������жϽ���Ƿ����㣬�Ӷ��������
		while (pParent)
		{
			if (pParent->_color == Black)//������Ǻڵ�,���õ���ֱ���˳�
			{
				break;
			}
			//��¼�游���Ͷ��屣��������
			PNode gparent = pParent->_pParent;
			PNode uncle = NULL;
			if (pParent == gparent->_pLeft)//�������ӽڵ���
			{
				uncle = gparent->_pRight;
				if (uncle&& uncle->_color == Red)//�����������Ϊ�����3,4
				{
					gparent->_color = Red;
					uncle->_color = pParent->_color = Black;
					pNewNode = gparent;//����gg���
					pParent = gparent->_pParent;
					continue;
				}
				else if (uncle == NULL || uncle->_color == Black)//���岻���ڻ���Ϊ��
				{
					if (pNewNode == pParent->_pLeft)//������
					{
						_RBRolateR(gparent);
						gparent->_color = Red;
						pParent->_color = Black;
					}
					else//�ڲ����
					{
						_RBRolateL(pParent);
						std::swap(pParent, pNewNode);//����pParent�Ͳ�����ָ���ֵ
						_RBRolateR(gparent);
						gparent->_color = Red;
						pParent->_color = Black;
					}
				}
				break;//ֱ������ѭ��
			}
			else//�ұߵ����
			{
				uncle = gparent->_pLeft;
				if (uncle&&uncle->_color == Red)//������ڶ���Ϊ��ɫ
				{
					gparent->_color = Red;
					uncle->_color = pParent->_color = Black;
					pNewNode = gparent;
					pParent = pNewNode->_pParent;
					continue;
				}
				else if (uncle == NULL || uncle->_color == Black)//���岻���ڻ���Ϊ��
				{
					if (pNewNode == pParent->_pRight)
					{
						_RBRolateL(gparent);
						gparent->_color = Red;
						pParent->_color = Black;
					}
					else
					{
						_RBRolateR(pParent);
						std::swap(pParent, pNewNode);
						_RBRolateL(gparent);
						gparent->_color = Red;
						pParent->_color = Black;
					}
				}
				break;
			}
		}
		_pRoot->_color = Black;
		return true;
	}
	void InOrder()
	{
		_InOrder(_pRoot);
	}
	bool IsRBTree()
	{
		if (_pRoot == NULL)//���ڵ� Ϊ���Ǻ����
			return true;
		if (_pRoot->_color == Red)//���ڵ�Ϊ��ɫ�϶����Ǻ����
			return false;
		int count = 0;//ͳ�ƺ�ɫ������Ŀ
		PNode cur = _pRoot;
		while (cur)//����һ���ο�·���ĺ�ɫ������Ŀ
		{
			if (cur->_color == Black)
				++count;
			cur = cur->_pLeft;
		}
		int k = 0;
		return _IsRBTree(_pRoot, count, k);
	}
private:
	bool _IsRBTree(PNode pRoot, int& count, int k)//�����K���ܴ�����
	{
		if (pRoot == NULL)
			return true;
		//�������������ĺ�ɫ�Ľ��
		if (pRoot->_pParent&&pRoot->_color == Red&&pRoot->_pParent->_color == Red)
			return false;
		//����Ǻ�ɫ���k++
		if (pRoot->_color == Black)
			k++;
		if (pRoot->_pLeft == NULL&&pRoot->_pRight == NULL)//�����Ҷ�ӽ��Ļ������ж�k��count
		{
			if (k == count)
				return true;
			else
				return false;
		}
		return _IsRBTree(pRoot->_pLeft, count, k) && _IsRBTree(pRoot->_pRight, count, k);
	}
	//�ҵ���ת
	void _RBRolateR(PNode parent)
	{
		if (NULL == parent)
			return;
		PNode SubL = parent->_pLeft;//
		PNode SubLR = SubL->_pRight;

		parent->_pLeft = SubLR;
		if (SubLR != NULL)
			SubLR->_pParent = parent;
		PNode pParent = parent->_pParent;
		SubL->_pRight = parent;
		parent->_pParent = SubL;
		if (pParent == NULL)
		{
			_pRoot = SubL;
			SubL->_pParent = NULL;
		}
		else
		{
			if (pParent->_pLeft == parent)
			{
				pParent->_pLeft = SubL;
				SubL->_pParent = parent;
			}
			else
			{
				pParent->_pRight = SubL;
				SubL->_pParent = parent;
			}
		}
	}
	void _RBRolateL(PNode parent)
	{
		if (NULL == parent)
			return;
		PNode SubR = parent->_pRight;
		PNode SubRL = SubR->_pLeft;
		parent->_pRight = SubRL;
		if (SubRL)
		{
			SubRL->_pParent = parent;
		}
		PNode pParent = parent->_pParent;
		SubR->_pLeft = parent;
		parent->_pParent = SubR;
		if (pParent == NULL)
		{
			_pRoot = SubR;
			SubR->_pParent = NULL;
		}
		else
		{
			if (pParent->_pLeft == parent)
			{
				pParent->_pLeft = SubR;
				SubR->_pParent = pParent;
			}
			else
			{
				pParent->_pRight = SubR;
				SubR->_pRight = pParent;
			}
		}
	}
	void _InOrder(PNode pRoot)
	{
		if (pRoot)
		{
			_InOrder(pRoot->_pLeft);
			cout << pRoot->_key << " ";
			_InOrder(pRoot->_pRight);
		}
	}
};
int main()
{
	RBTree<int, int> s;
	int arr[] = { 5, 3, 7, 1, 4, 6, 13, 8, 15, 12 };
	int size = sizeof(arr) / sizeof(*arr);
	for (int i = 0; i < size; i++)
		s.Insert(arr[i], i);
	s.InOrder();
	cout << s.IsRBTree();
	system("pause");
	return 0;
}
