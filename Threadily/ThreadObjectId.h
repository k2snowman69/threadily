#pragma once
#include <stdexcept>

#include "IThreadObjectId.h"

namespace threadily {
	/// Represents an Id of a ThreadObject instance. Created so that
	/// Objects with complicated id types (multiple reference ids) can be
	/// represented by either subclassing or implementing similar methods. For an example, look at Threadily.SampleObjects
	/// for any struct which subclasses ThreadObjectId
	struct ThreadObjectId : public IThreadObjectId {
		// internal id used to differentiate all objects that a ThreadObjectManager deals with
		unsigned int instanceId;

		// Default constructor
		ThreadObjectId() {
			// Doesn't do anything as this should be an invalid state. It may get called though
		}
		// Copy constructor
		ThreadObjectId(const ThreadObjectId& other) {
			this->instanceId = other.instanceId;
		}
		// Shortcut to go from an object with a single id
		ThreadObjectId(unsigned int instanceId) {
			this->instanceId = instanceId;
		}
		// Shortcuts to revert back to an int
		// TODO see if these are useful or not
		virtual operator unsigned int&() { return this->instanceId; }
		virtual operator unsigned int() const { return this->instanceId; }

		// Override < so that comparisons for maps work
		bool operator < (const ThreadObjectId& other) const {
			return this->instanceId < other.instanceId;
		};
	};
}