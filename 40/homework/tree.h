#ifndef TREE_H
#define TREE_H

template< typename T >
class CBTree
{
	struct SNode
	{
		T mData;
		SNode *mLeft;
		SNode *mRight;

		SNode( ) :
			mLeft( NULL ),
			mRight( NULL )
		{ }
	};

	static void delete_tree( SNode *nNode )
	{
		if( !nNode )
			return;

		if( nNode->mLeft )
			CBTree::delete_tree( nNode->mLeft );

		if( nNode->mRight )
			CBTree::delete_tree( nNode->mRight );

		delete nNode;
	}

	static SNode *copy_tree( SNode *nSource )
	{
		if( !nSource )
			return NULL;
		
		SNode *tDestination = new SNode;
		tDestination->mData = nSource->mData;

		if( nSource->mLeft )
			tDestination->mLeft = CBTree::copy_tree( nSource->mLeft );

		if( nSource->mRight )
			tDestination->mRight = CBTree::copy_tree( nSource->mRight );

		return tDestination;
	}

	SNode *m_Root;

public:
	CBTree( ) :
		m_Root( NULL )
	{ }
	
	CBTree( T nRoot ) :
		m_Root( new SNode )
	{
		this->m_Root->mData = nRoot;
	}

	CBTree( const CBTree &nOther ) :
		m_Root( CBTree::copy_tree( nOther.m_Root ) )
	{ }

	~CBTree( )
	{
		CBTree::delete_tree( this->m_Root );
	}

	CBTree &operator=( const CBTree &nOther )
	{
		if( this != &nOther )
		{
			CBTree::delete_tree( this->m_Root );
			this->m_Root = CBTree::copy_tree( nOther.m_Root );
		}

		return *this;
	}

	static CBTree Create3( T nRoot,
		const CBTree *nLeft, const CBTree *nRight )
	{
		CBTree tResult = CBTree( nRoot );

		if( nLeft )
			tResult.m_Root->mLeft = CBTree::copy_tree( nLeft->m_Root );

		if( nRight )
			tResult.m_Root->mRight = CBTree::copy_tree( nRight->m_Root );

		return tResult;
	}

	bool empty( ) const
	{
		return this->m_Root == NULL;
	}

	const T &root( ) const
	{
		return this->m_Root->mData;
	}

	T &root( )
	{
		if( !this->m_Root )
			this->m_Root = new SNode;

		return this->m_Root->mData;
	}

	bool has_left( ) const
	{
		return this->m_Root->mLeft != NULL;
	}

	const T &left( ) const
	{
		return this->m_Root->mLeft->mData;
	}

	T &left( )
	{
		if( !this->m_Root->mLeft )
			this->m_Root->mLeft = new SNode;

		return this->m_Root->mLeft->mData;
	}

	bool has_right( ) const
	{
		return this->m_Root->mRight != NULL;
	}

	const T &right( ) const
	{
		return this->m_Root->mRight->mData;
	}

	T &right( )
	{
		if( !this->m_Root->mRight )
			this->m_Root->mRight = new SNode;

		return this->m_Root->mRight->mData;
	}

	CBTree<T> left_tree( ) const
	{
		CBTree<T> tResult;
		tResult.m_Root = CBTree::copy_tree( this->m_Root->mLeft );
		return tResult;
	}

	CBTree<T> right_tree( ) const
	{
		CBTree<T> tResult;
		tResult.m_Root = CBTree::copy_tree( this->m_Root->mRight );
		return tResult;
	}

};

#endif
