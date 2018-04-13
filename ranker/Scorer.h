
#ifndef EECS398_SEARCH_SCORER_H
#define EECS398_SEARCH_SCORER_H

#include "Site.h"
#include <unordered_map>
#include <string>
#include <vector>

/**
 * Custom Comparator to find the minimum frequency
 */
typedef std::pair<std::string, unsigned long > MyPairType;
struct CompFreq
	{
	bool operator()(const MyPairType& left, const MyPairType& right) const
		{
		return left.second < right.second;
		}
	};

/**
 * Gets the data need to compute the score
 *
 * @param wordData
 * @param queryTokens
 * @param minLength
 * @param minLengthWord
 * @return
 */

struct ScoreData
	{
	unsigned long avrgSpanDelta;
	int numSpans;
	int numPhrases;

	ScoreData( std::unordered_map< std::string, data>* wordData, std::vector< std::string > *queryTokens, unsigned long minLength, int minLengthWord )
		{
		// get average span delta
		unsigned long avrgSpanDelta = 0;
		unsigned long spanDelta = 0;
		int numPhrases = 0;
		int numSpans = 0;

		int row = 0;
		//while the min delta for each word column has not reached the end of the row
		while ( ( *wordData )[ ( *queryTokens )[ minLengthWord ] ].minDelta + row  < minLength )
			{
			//find the delta between each word in the query and the next
			for ( int col = 0; col < queryTokens->size( ) - 1; ++col )
				{
				int indexA = ( *wordData )[ ( *queryTokens )[ col ] ].minDelta + row;
				int indexB = ( *wordData )[ ( *queryTokens )[ col + 1 ] ].minDelta + row;
				long delta = std::abs ( long ( ( *wordData )[ ( *queryTokens )[ col ] ].offsets[ indexA ] - ( *wordData )[ ( *queryTokens )[ col + 1 ] ].offsets[ indexB ] ) );
				spanDelta += delta;
				avrgSpanDelta += delta;
				}
			++numSpans;
			++row;

			// check if exact phrase match
			if ( spanDelta == queryTokens->size( ) - 1 )
				++numPhrases;

			}
		avrgSpanDelta /= numSpans;

		this->avrgSpanDelta = avrgSpanDelta;
		this->numSpans = numSpans;
		this->numPhrases = numPhrases;

		}

	};

class Scorer
	{
public:
	/**
	 * Scorer cstor
	 */
	Scorer( );

	/***
	 * Calculate the score for some site, Normalize the score to 1.0
	 *
	 * @return
	 */
	double getScore( Site );

	/**
	 * Map of domain TLDs to weights
	 *
	 * https://en.wikipedia.org/wiki/List_of_Internet_top-level_domains
	 */
	std::unordered_map< std::string, double > domainMap =
			{ { ".gov", 0.9 }, { ".com", 0.6 }, { ".net", 0.4 }, { ".org", 1.0 }, { ".edu", 0.9 }, { ".us", 0.1 } };

	std::string getUrlDomain( std::string url );

	/**
	 * Static ranker
	 *
	 * @param inputSite
	 * @return double
	 */
	double staticScore ( Site inputSite );

	/**
	 * Calculates score for exact phrase matches
	 *
	 * @param inputSite
	 * @return double
	 */
//	double phraseMatch ( Site inputSite );

//	int phraseMatchHelper ( std::string str1, std::string str2, std::unordered_map< std::string, data > *wordData );

	/**
	 * Calculates score for proximity matches
	 *
	 * @param inputSite
	 * @return double
	 */
	double proximityMatch ( Site inputSite );

	/**
	 * returns the word with the min frequency
	 * @return
	 */
	std::string getMinFreq( std::unordered_map< std::string, data>* wordData, std::vector< std::string > *queryTokens );

	int getMinDelta( unsigned long start, std::vector< size_t >* offsets );

	std::pair< unsigned long, int > setMinDelta( std::unordered_map< std::string, data>* wordData, std::vector< std::string > *queryTokens, unsigned long start );


private:
	const double STATIC_WEIGHT = 1.0;
	const double PHRASE_WEIGHT = 1.0;
	const double PROXIMITY_WEIGHT = 1.0;
	const double TFIDF_WEIGHT = 1.0;
	const double ALPHA = 3;
	const double ALPHA_PRIME = 10;

	};
#endif //EECS398_SEARCH_SCORER_H
