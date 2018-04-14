
#include "../Scorer.h"
#include "../Site.h"
#include <iostream>
#include <vector>
#include <cassert>

void testStaticScore( );
void testProximityMatchSimple( );
void testProximityMatchOneWord ( );
void testPhraseMatchSymbols ( );
void testProxMultipleOffsets ( );

int main( )
	{
	cout << "------Starting Scorer Test------" << endl << endl;

	testStaticScore( );
	testProximityMatchSimple( );
	testProximityMatchOneWord( );
	testPhraseMatchSymbols ( );
	testProxMultipleOffsets( );

	cout << "------Passed All Scorer Tests--- :)" << endl;
	}


void testStaticScore( )
	{
	cout << "Testing Static Score....." << endl;
	Query query( "banana cream pie" );
	std::string url = "https://www.tasteofhome.com/recipes/favorite-banana-cream-pie";
	Site newSite( url, query, "TITLE" );

	//.com
	Scorer scorer = Scorer();
	auto score = scorer.staticScore( newSite );
	cout << "Score: " << score << endl;
	assert( score == 0.6 );

	//.edu & no protocol
	url = "umich.edu/";
	Site newSite2( url, query, "TITLE" );
	score = scorer.staticScore( newSite2 );
	cout << "Score: " << score << endl;
	assert( score == 0.9 );

	//.gov
	url = "https://www.whitehouse.gov/";
	Site newSite3( url, query, "TITLE" );
	score = scorer.staticScore( newSite3 );
	cout << "Score: " << score << endl;
	assert( score == 0.9 );

	//.net
	url = "testwebsite.net";
	Site newSite4( url, query, "TITLE" );
	score = scorer.staticScore( newSite4 );
	cout << "Score: " << score << endl;
	assert( score == 0.4 );

	//.us
	url = "http://testing.us";
	Site newSite5( url, query, "TITLE" );
	score = scorer.staticScore( newSite5 );
	cout << "Score: " << score << endl;
	assert( score == 0.1 );

	//other
	url = "umich.biz/";
	Site newSite6( url, query, "TITLE" );
	score = scorer.staticScore( newSite6 );
	cout << "Score: " << score << endl;
	assert( score == 0.0 );

	cout << "Static Score Passed!" << endl << endl;
	}


void testProximityMatchSimple( )
	{
	cout << "Testing Proximity Match....." << endl;

	Query query( "banana cream pie" );
	std::string url( "https://www.tasteofhome.com/recipes/favorite-banana-cream-pie" );
	Site newSite( url, query, "TITLE" );

	/// Banana Cream Pie
	newSite.wordData[ "banana"].frequency = 1;
	newSite.wordData[ "cream"].frequency = 1;
	newSite.wordData[ "pie"].frequency = 1;

	newSite.wordData[ "banana"].offsets.push_back( 0 );
	newSite.wordData[ "cream"].offsets.push_back( 1 );
	newSite.wordData[ "pie"].offsets.push_back( 2 );

	Scorer scorer = Scorer( );
	auto score = scorer.proximityMatch( newSite );
	cout << "Score: " << score << endl;
	assert( score == 1 );

	/// Banana Strawberry Cream Pie

	newSite.wordData[ "strawberry"].frequency = 1;

	newSite.wordData[ "strawberry"].offsets.push_back( 0 );
	newSite.wordData[ "cream"].offsets[ 0 ] = 2;
	newSite.wordData[ "pie"].offsets[ 0 ] = 3;

	score = scorer.proximityMatch( newSite );
	cout << "Score: " << score << endl;
	assert( score == ( 3 / 14.5) );

	/// Banana Strawberry Cream Cake Pie
	newSite.wordData[ "cake"].frequency = 1;

	newSite.wordData[ "cake"].offsets.push_back( 3 );
	newSite.wordData[ "pie"].offsets[ 0 ] = 4;

	score = scorer.proximityMatch( newSite );
	cout << "Score: " << score << endl;
	assert( score == ( 2.25 / 14.5 ) );

	cout << "Proximity Match Passed!" << endl << endl;

	}

void testProximityMatchOneWord ( )
	{

	cout << "Testing Proximity Match One Word....." << endl;

	Query query( "Cream" );
	std::string url( "https://www.tasteofhome.com/recipes/favorite-banana-cream-pie" );
	Site newSite( url, query, "TITLE" );

	/// Banana Cream Pie
	newSite.wordData[ "banana"].frequency = 1;
	newSite.wordData[ "cream"].frequency = 1;
	newSite.wordData[ "cake"].frequency = 1;

	newSite.wordData[ "banana"].offsets.push_back( 0 );
	newSite.wordData[ "cream"].offsets.push_back( 1 );
	newSite.wordData[ "cake"].offsets.push_back( 2 );

	Scorer scorer = Scorer();
	auto score = scorer.proximityMatch( newSite );

	cout << "Score: " << score << endl;
	assert( score == 0 );

	cout << "Proximity Match One Word Passed!" << endl << endl;

	}

void testPhraseMatchSymbols ( )
	{
	cout << "Testing Proximity Match Symbols....." << endl;

	Query query( "$Cream@ Pie!" );
	std::string url( "https://www.tasteofhome.com/recipes/favorite-banana-cream-pie" );
	Site newSite( url, query, "TITLE" );

	/// Banana Cream Pie Cake
	newSite.wordData[ "banana"].frequency = 1;
	newSite.wordData[ "cream"].frequency = 1;
	newSite.wordData[ "pie"].frequency = 1;
	newSite.wordData[ "cake"].frequency = 1;

	newSite.wordData[ "banana"].offsets.push_back( 0 );
	newSite.wordData[ "cream"].offsets.push_back( 1 );
	newSite.wordData[ "pie"].offsets.push_back( 2 );
	newSite.wordData[ "cake"].offsets.push_back( 3 );

	Scorer scorer = Scorer();
	auto score = scorer.proximityMatch( newSite );

	cout << "Score: " << score << endl;
	assert( score == 1 );

	cout << "Proximity Match Symbols Passed!" << endl << endl;

	}

void testProxMultipleOffsets ( )
	{

	cout << "Testing Proximity Match Mult Offsets....." << endl;

	Query query( "Banana Cream Pie" );
	std::string url( "https://www.tasteofhome.com/recipes/favorite-banana-cream-pie" );
	Site newSite( url, query, "TITLE" );

	/// Banana Banana Cream Cream Pie Pie
	newSite.wordData[ "banana"].frequency = 2;
	newSite.wordData[ "cream"].frequency = 2;
	newSite.wordData[ "pie"].frequency = 2;

	newSite.wordData[ "banana"].offsets = { 0, 1 };
	newSite.wordData[ "cream"].offsets = { 2, 3 };
	newSite.wordData[ "pie"].offsets = { 4, 5 };

	Scorer scorer = Scorer();
	auto score = scorer.proximityMatch( newSite );

	cout << "Score: " << score << endl;
	assert( score == ( 2.25 / 14.5 ) );

	/// Banana Pie Cream Banana Cream Pie Pie Random Random Cream Banana Random Random Cream Random Pie
	///   0     1     2     3     4     5  6    7      8     9     10       11    12    13    14-19  20-26
	newSite.wordData[ "banana"].frequency = 3;
	newSite.wordData[ "cream"].frequency = 4;
	newSite.wordData[ "pie"].frequency = 7;

	newSite.wordData[ "banana"].offsets = { 0, 3, 10 };
	newSite.wordData[ "cream"].offsets = { 2, 4, 9, 13 };
	newSite.wordData[ "pie"].offsets = { 1, 5, 6, 20, 24, 25, 26 };

	score = scorer.proximityMatch( newSite );

	cout << "Score: " << score << endl;
	auto math = ( ( 6.0 + ( 1.0 / 3.0 ) ) / 14.5 ) ;
	assert( score == math );

	cout << "Proximity Match Mult Offsets Passed!" << endl << endl;

	}

