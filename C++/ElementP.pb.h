// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ElementP.proto

#ifndef PROTOBUF_ElementP_2eproto__INCLUDED
#define PROTOBUF_ElementP_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace proto {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_ElementP_2eproto();
void protobuf_AssignDesc_ElementP_2eproto();
void protobuf_ShutdownFile_ElementP_2eproto();

class ElementP;

// ===================================================================

class ElementP : public ::google::protobuf::Message {
 public:
  ElementP();
  virtual ~ElementP();

  ElementP(const ElementP& from);

  inline ElementP& operator=(const ElementP& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ElementP& default_instance();

  void Swap(ElementP* other);

  // implements Message ----------------------------------------------

  ElementP* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ElementP& from);
  void MergeFrom(const ElementP& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 position = 1;
  inline bool has_position() const;
  inline void clear_position();
  static const int kPositionFieldNumber = 1;
  inline ::google::protobuf::int32 position() const;
  inline void set_position(::google::protobuf::int32 value);

  // required double value = 2;
  inline bool has_value() const;
  inline void clear_value();
  static const int kValueFieldNumber = 2;
  inline double value() const;
  inline void set_value(double value);

  // @@protoc_insertion_point(class_scope:proto.ElementP)
 private:
  inline void set_has_position();
  inline void clear_has_position();
  inline void set_has_value();
  inline void clear_has_value();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  double value_;
  ::google::protobuf::int32 position_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_ElementP_2eproto();
  friend void protobuf_AssignDesc_ElementP_2eproto();
  friend void protobuf_ShutdownFile_ElementP_2eproto();

  void InitAsDefaultInstance();
  static ElementP* default_instance_;
};
// ===================================================================


// ===================================================================

// ElementP

// required int32 position = 1;
inline bool ElementP::has_position() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ElementP::set_has_position() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ElementP::clear_has_position() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ElementP::clear_position() {
  position_ = 0;
  clear_has_position();
}
inline ::google::protobuf::int32 ElementP::position() const {
  return position_;
}
inline void ElementP::set_position(::google::protobuf::int32 value) {
  set_has_position();
  position_ = value;
}

// required double value = 2;
inline bool ElementP::has_value() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ElementP::set_has_value() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ElementP::clear_has_value() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ElementP::clear_value() {
  value_ = 0;
  clear_has_value();
}
inline double ElementP::value() const {
  return value_;
}
inline void ElementP::set_value(double value) {
  set_has_value();
  value_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace proto

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_ElementP_2eproto__INCLUDED