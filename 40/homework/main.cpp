#include <iostream>
#include <string>
#include <cstdlib>
#include <stack>
#include <list>
#include <cstring>
#include "tree.h"

struct SMathElement
{
	const static int sPriority[4];

	double mValue;
	enum ESign { SIGN_NONE,
		SIGN_PLUS, SIGN_MINUS, SIGN_MULTIPLY, SIGN_DIVIDE } mSign;

	static ESign get_sign( char nSign )
	{
		switch( nSign )
		{
			case '+':
				return SIGN_PLUS;

			case '-':
				return SIGN_MINUS;

			case '*':
				return SIGN_MULTIPLY;

			case '/':
				return SIGN_DIVIDE;

			default:
				throw 0;
		}
	}
};

const int SMathElement::sPriority[4] = { 1, 1, 10, 10 };

double calculate( const CBTree<SMathElement> &nTree )
{
	if( nTree.empty( ) )
		throw 0;

	if( !nTree.has_left( ) )
	{
		return nTree.root( ).mValue;
	}

	double tLeftValue = calculate( nTree.left_tree( ) );
	double tRightValue = calculate( nTree.right_tree( ) );

	switch( nTree.root( ).mSign )
	{
		case SMathElement::SIGN_PLUS:
			return tLeftValue + tRightValue;

		case SMathElement::SIGN_MINUS:
			return tLeftValue - tRightValue;

		case SMathElement::SIGN_MULTIPLY:
			return tLeftValue * tRightValue;

		case SMathElement::SIGN_DIVIDE:
			return tLeftValue / tRightValue;

		default:
			throw 0;
	}
}

CBTree< SMathElement > parse( std::string nInput )
{
	std::stack< char > tStack;
	std::list< CBTree< SMathElement > > tTokens;
	SMathElement tElement;

	const char *tExpression = nInput.c_str( );
	char *tDummy;
	bool tNextSign = false;

	while( 1 )
	{
		if( tNextSign )
		{
			tExpression = strpbrk( tExpression, "+-*/()" );
			if( !tExpression )
				break;

			if( *tExpression == '(' || ( *tExpression != ')' &&
				( tStack.empty( ) || tStack.top( ) == '(' ) ) )
			{
		    	tStack.push( *tExpression );
		    }
		    else if( *tExpression == ')' )
		    {
		    	while( tStack.top( ) != '(' )
		    	{
		    		tElement.mSign = SMathElement::get_sign( tStack.top( ) );
			    	tTokens.push_back( CBTree< SMathElement >( tElement ) );

		    		tStack.pop( );
		    	}

	    		tStack.pop( );
				tNextSign = 0;
		    }
		    else
		    {
		    	SMathElement::ESign tSign = SMathElement::get_sign(
		    		*tExpression );
		    	int tSignPriority = SMathElement::sPriority[tSign];

		    	SMathElement::ESign tStackSign =
		    		SMathElement::get_sign( tStack.top( ) );
		    	int tStackSignPriority =
		    		SMathElement::sPriority[tStackSign];

		    	if( tSignPriority > tStackSignPriority )
		    	{
		    		tStack.push( *tExpression );
		    	}
		    	else if( tSignPriority == tStackSignPriority )
		    	{
		    		tElement.mSign = tStackSign;
		    		tTokens.push_back(
		    			CBTree< SMathElement >( tElement ) );
		    		
		    		tStack.pop( );
		    		tStack.push( *tExpression );
		    	}
		    	else
		    	{
		    		tElement.mSign = tStackSign;
		    		tTokens.push_back(
		    			CBTree< SMathElement >( tElement ) );

		    		tStack.pop( );

		    		tNextSign = 1;
		    		continue;
		    	}
		    }

			++tExpression;
		}
		else
		{
			tElement.mValue = strtod( tExpression, &tDummy );
			tExpression = tDummy;
			
			if( *tExpression != '(' )
			{
		    	tElement.mSign = SMathElement::SIGN_NONE;
		    	tTokens.push_back( CBTree< SMathElement >( tElement ) );
	    	}
		}

		tNextSign = !tNextSign;
	}

	while( !tStack.empty( ) )
	{
		tElement.mSign = SMathElement::get_sign( tStack.top( ) );
   		tTokens.push_back( CBTree< SMathElement >( tElement ) );

		tStack.pop( );
	}


	for( std::list< CBTree< SMathElement > >::iterator tIterator =
		tTokens.begin( ); tIterator != tTokens.end( ); ++tIterator )
	{
		if( tIterator->root( ).mSign != SMathElement::SIGN_NONE )
		{
			SMathElement tRoot = tIterator->root( );
			CBTree< SMathElement > *tRightTree = &( *( --tIterator ) );
			CBTree< SMathElement > *tLeftTree = &( *( --tIterator ) );

			++( ++tIterator );
			*tIterator = CBTree< SMathElement >::Create3( tRoot,
				tLeftTree, tRightTree );

			tIterator = tTokens.erase( --tIterator );
			tIterator = tTokens.erase( --tIterator );
		}
	}

	return tTokens.front( );
}

int main( )
{
	std::string tInput;
	std::getline( std::cin, tInput );
	CBTree<SMathElement> b = parse( tInput );

	std::cout << calculate( b ) << std::endl;
	return 0;
}
