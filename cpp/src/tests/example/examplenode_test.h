#pragma once

#include "examplenode.h"
#include <log.h>
#include <tests/typedef.h>

class ExampleNodeTestSuite : public TestSuite {
	TEST_BODY(ExampleNodeTestSuite) {
		return {
			//
			memnew(ExampleNodeTestSuite::simplecall),
		};
	}

	TEST_CASE(ExampleNodeTestSuite, simplecall, {
		auto* othernode = memnew(ExampleNode);
		othernode->simple_call();
	})
};
