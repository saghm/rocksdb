// Copyright (c) 2013 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_ROCKSDB_INCLUDE_COMPACTION_FILTER_H_
#define STORAGE_ROCKSDB_INCLUDE_COMPACTION_FILTER_H_

#include <string>

namespace rocksdb {

class Slice;

// CompactionFilter allows an application to modify/delete a key-value at
// the time of compaction.

class CompactionFilter {
 public:
  virtual ~CompactionFilter() {}


  // The compaction process invokes this
  // method for kv that is being compacted. A return value
  // of false indicates that the kv should be preserved in the
  // output of this compaction run and a return value of true
  // indicates that this key-value should be removed from the
  // output of the compaction.  The application can inspect
  // the existing value of the key and make decision based on it.
  //
  // When the value is to be preserved, the application has the option
  // to modify the existing_value and pass it back through new_value.
  // value_changed needs to be set to true in this case.
  virtual bool Filter(int level,
                      const Slice& key,
                      const Slice& existing_value,
                      std::string* new_value,
                      bool* value_changed) const = 0;

  // Returns a name that identifies this compaction filter.
  // The name will be printed to LOG file on start up for diagnosis.
  virtual const char* Name() const = 0;
};

// Each compaction will create a new CompactionFilter allowing the
// application to know about different campactions
class CompactionFilterFactory {
  public:
    virtual ~CompactionFilterFactory() { };
    virtual std::unique_ptr<CompactionFilter> CreateCompactionFilter() = 0;

    // Returns a name that identifies this compaction filter factory.
    virtual const char* Name() const = 0;
};

// Default implementaion of CompactionFilterFactory which does not
// return any filter
class DefaultCompactionFilterFactory : public CompactionFilterFactory {
  public:
    virtual std::unique_ptr<CompactionFilter>
    CreateCompactionFilter() override {
      return std::unique_ptr<CompactionFilter>(nullptr);
    }

    virtual const char* Name() const override {
      return "DefaultCompactionFilterFactory";
    }
};

}  // namespace rocksdb

#include "rocksdb/rocksdb_to_leveldb.h"

#endif  // STORAGE_ROCKSDB_INCLUDE_COMPACTION_FILTER_H_