#pragma once

namespace IndexerConstants {
//    const string pathToIndex = "/buildIndex/";
	const string pathToIndex = "/constraintSolver/index-test-files/twitter/";

	const size_t maxWordSize = 30;

    const size_t masterKeySize = 30;
    const size_t masterValueSize = 168;

    const size_t chunkSeekKeySize = 30;
    const size_t chunkSeekValueSize = 8;

    const size_t chunkWordSeekKeySize = 30;
    const size_t chunkWordSeekValueSize = 168;

    const size_t chunkSizeLimit = 20000;

    const size_t saveEveryXEntries = 100;
}