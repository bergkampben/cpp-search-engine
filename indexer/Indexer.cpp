#include "Indexer.h"


#define  pathToIndex "/build/"

Indexer::Indexer( ProducerConsumerQueue < DocIndex * > *doc_index_queue_in,
						ProducerConsumerQueue < unordered_map < string, DocIndex * > > *anchor_in
) :
		pointerToDictionaries( doc_index_queue_in ), AnchorQueue( anchor_in )
	{
	totalWordsIndexed = 0;
	currentFile = 0;
	currentlyIndexed = 0;
	currentBlockNumberWords = 0;
	currentBlockNumberDocs = 0;

	}

void Indexer::run()
	{

	int documentsStored = 0;
	while ( *alive  || pointerToDictionaries->Size( ) > 0 )
		{

		if( pointerToDictionaries->Size( ) > 0)
			{
			cout << "Received number of  " << documentsStored << endl;
			documentsStored++;

			DocIndex *dictionary = pointerToDictionaries->Pop( );

			DocumentEnding docEnd = DocumentEnding( );
			size_t indexedCount = 0;
			currentBlockNumberDocs++;

			for ( auto word : *dictionary )
				{
				if ( word.first.at( 0 ) == '=' )
					{
					docEnd.url = word.first.substr( 1, word.first.length( ));
					continue;
					}

				indexedCount += word.second.size( );
				currentBlockNumberWords += word.second.size( );
				totalWordsIndexed += word.second.size( );

				for ( auto location : word.second )
					{
					masterDictionary[ word.first ].push_back( currentlyIndexed + location );
					}
				}

			currentlyIndexed += indexedCount;
			docEnd.docEndPosition = currentlyIndexed;
			docEnd.docNumWords = indexedCount;
			docEndings.push_back( docEnd );
			//add the url to the ->doc end map
			urlToDocEndings[ docEnd.url ] = docEnd.docEndPosition;


			if ( currentBlockNumberWords >= 20000 )
				{
				cout << " --- Saving current chunk --- " << endl;
				save( );
				saveWordSeek( );
				reset( );
				}
			delete dictionary;
			}


		}
	cout << "Indexer is shutting down" << endl;
	//cout << "Size of ptr 2 dict" << pointerToDictionaries->Size( );
	//cout << "alive state: " << *alive;
	save( );
	saveWordSeek( );
	reset( );
	saveChunkDictionary( );

	unordered_map < string, DocIndex * > anchorDict = AnchorQueue->Pop( );
	SaveAnchorText( &anchorDict );
	cout << " Indexer has finished running" << endl;
	return;
	}

void Indexer::verbose_run()
	{
	/*
    while(pointerToDictionaries.Size() != 0) {
		 	DocIndex *pointerToDictionaries.Pop();
        for(auto word : dictionary) {
	        for(auto location : word.second) {
//                indexedCount++;
                masterDictionary[word.first].push_back(location);
                }
            }
        }
        */
	}

void Indexer::save()
	{

	MMDiskHashTable seeker( util::GetCurrentWorkingDir( ) + pathToIndex + to_string( currentFile ) + "-seek.txt", 30, 8 );
	string fileName = util::GetCurrentWorkingDir( ) + pathToIndex + to_string( currentFile ) + ".txt";
	int file = open( fileName.c_str( ), O_CREAT | O_WRONLY, S_IRWXU );

	// TODO: these should really be c strings
	string statsHeader = "===STATS==="
										"\nunique words: " + to_string( masterDictionary.size( )) +
								"\nnumber words: " + to_string( currentBlockNumberWords ) +
								"\nnumber docs: " + to_string( currentBlockNumberDocs ) +
								"\n===========\n";
	write( file, statsHeader.c_str( ), strlen( statsHeader.c_str( )));

	// REALLY GROSS HACK
	size_t seekOffset = strlen( statsHeader.c_str( ));

	for ( auto word : masterDictionary )
		{
		if ( word.first.size( ) > 30 )
			{
			string resized = word.first;
			resized.resize( 30 );
			seeker.insert( resized, to_string( seekOffset ));
			}
		else
			{
			seeker.insert( word.first, to_string( seekOffset ));
			}

		chunkDictionary[ word.first ].first.push_back( currentFile );
//        string wordBreak = word.first + "\n";
//        write(file, wordBreak.c_str(), strlen(wordBreak.c_str()));
//        seekOffset += strlen(wordBreak.c_str());
		bool firstPost = true;
		size_t lastOne = 0;
		int numIndexed = 0;
		for ( auto location : word.second )
			{
			chunkDictionary[ word.first ].second++;
			numIndexed++;
			if ( numIndexed == 100 )
				{
				PostingsSeekTableEntry entry = PostingsSeekTableEntry( );
				entry.offset = seekOffset;
				entry.realLocation = location;
				postingsSeekTable[ word.first ].push_back( entry );
				numIndexed = 0;
				}
			if ( firstPost )
				{
				string locationSpace = to_string( location ) + " ";
				write( file, locationSpace.c_str( ), strlen( locationSpace.c_str( )));
				seekOffset += strlen( locationSpace.c_str( ));
				firstPost = false;
				}
			else
				{
				size_t delta = location - lastOne;
				string deltaSpace = to_string( delta ) + " ";
				write( file, deltaSpace.c_str( ), strlen( deltaSpace.c_str( )));
				seekOffset += strlen( deltaSpace.c_str( ));
				}
			lastOne = location;
			}
		write( file, "\n", 1 );
		seekOffset += 1;
		}

	string docEndingHeader = "===Document Endings===\n";
	write( file, docEndingHeader.c_str( ), strlen( docEndingHeader.c_str( )));
	seekOffset += strlen( docEndingHeader.c_str( ));
	seeker.insert( "=docEnding", to_string( seekOffset ));
	int docEndSeekCounter = 0; // save seek every 100 doc ends in the chunk
	for ( auto ending : docEndings )
		{
		string docEndString = "[" +
									 ending.url + ", " +
									 to_string( ending.docEndPosition ) + ", " +
									 to_string( ending.docNumWords ) + "]\n";
		write( file, docEndString.c_str( ), strlen( docEndString.c_str( )));
		docEndSeekCounter++;
		if ( docEndSeekCounter == 100 )
			{
			docEndSeekCounter = 0;
			docEndingsSeek.push_back( {ending.docEndPosition, seekOffset} );
			}
		seekOffset += strlen( docEndString.c_str( ));
		}

	close( file );
	seeker.CloseFile();
	}

void Indexer::saveChunkDictionary()
	{

	MMDiskHashTable dhtChunk = MMDiskHashTable( util::GetCurrentWorkingDir( ) + pathToIndex + "master.txt", 30, 168 );
	for ( auto word : chunkDictionary )
		{
		string key = word.first;
		if ( key.size( ) > 30 )
			{
			key.resize( 30 );
			}
		string value = "";
		for ( auto chunk : word.second.first )
			{
			value += to_string( chunk ) + " ";
			}
		value += "\t" + to_string( word.second.second );
		dhtChunk.insert( key, value );
		}
	dhtChunk.insert( "=totalNumberIndexed", to_string( totalWordsIndexed ));
	}

void Indexer::saveWordSeek()
	{
	MMDiskHashTable wordSeek = MMDiskHashTable(
			util::GetCurrentWorkingDir( ) + pathToIndex + to_string( currentFile ) + "-wordseek.txt", 30, 168 );
	for ( auto word : postingsSeekTable )
		{
		string key = word.first;
		if ( key.size( ) > 30 )
			{
			key.resize( 30 );
			}
		string value = "";
		for ( auto entry : word.second )
			{
			value += ("<" + to_string( entry.offset ) + ", " + to_string( entry.realLocation ) + "> ");
			}
		wordSeek.insert( key, value );
		}
	string key = "=docEnding";
	string value = "";
	int currentEndingPartition = 0;
	for ( size_t i = 0; i < docEndingsSeek.size( ); i++ )
		{
		string prospectiveDocEnding = "<" +
												to_string( docEndingsSeek[ i ].first ) +
												", " + to_string( docEndingsSeek[ i ].second ) + "> ";
		if ( value.size( ) + prospectiveDocEnding.size( ) <= 168 )
			{
			value += prospectiveDocEnding;
			}
		else
			{
			wordSeek.insert( key + to_string( currentEndingPartition ), value );
			currentEndingPartition++;
			value = prospectiveDocEnding;
			}
		}
	}

void Indexer::verbose_save()
	{
	map < string, vector < size_t > > maps( masterDictionary.begin( ), masterDictionary.end( ));
	for ( auto word : maps )
		{
		cout << word.first << endl;
		for ( auto location : word.second )
			{
			cout << location << " ";
			}
		cout << endl;
		}
	currentFile++;
	}

void Indexer::reset()
	{
	masterDictionary.clear( );
	docEndings.clear( );
	postingsSeekTable.clear( );
	docEndingsSeek.clear( );
	currentBlockNumberWords = 0;
	currentBlockNumberDocs = 0;

	}


void Indexer::Kill()
	{
	*(this->alive) = false;
	currentFile++;
	}


void Indexer::SaveAnchorText( unordered_map < string, DocIndex * > *anchorDict )
	{

	//TODO create pointer to anchor


	//pointerToAnchor->Pop();
	//pass a dictionary of
	//map <url string> - >  vector<anchor word>
	//for each url in map
	//look up url string in  url -> docEnding map
	//for each anchor text in url map
	// create a  anchor text - > list of doc endings
	//write to disk

	cout << " -- SAVING ANCHOR TEXT --- " << endl;
	for ( auto const &ent1 : *anchorDict )
		{
		auto const &outer_key = ent1.first;
		cout << "url: " << outer_key << endl;

		if ( urlToDocEndings.find( outer_key ) != urlToDocEndings.end( ))
			{
			size_t docEndForUrl = urlToDocEndings[ outer_key ];
			cout << "Urls doc end : " << docEndForUrl << endl;

			}


		DocIndex *inner_map = ent1.second;

		for ( auto const &ent2 : *inner_map )
			{


			auto const &inner_key = ent2.first;
			auto const &inner_value = ent2.second;
			//cout << "url: " << outer_key << endl;
			//cout << "anchor text : " << inner_key << endl;
			//for(auto offset :inner_value)
			//	cout << "offset " << offset << endl;


			}

		}


	}


