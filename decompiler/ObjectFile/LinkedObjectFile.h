#pragma once

/*!
 * @file LinkedObjectFile.h
 * An object file's data with linking information included.
 */

#ifndef NEXT_LINKEDOBJECTFILE_H
#define NEXT_LINKEDOBJECTFILE_H

#include <cstdint>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "LinkedWord.h"
#include "decompiler/Function/Function.h"
#include "decompiler/util/LispPrint.h"

/*!
 * A label to a location in this object file.
 * Doesn't have to be word aligned.
 */
struct Label {
  std::string name;
  int target_segment;
  int offset;  // in bytes
};

/*!
 * An object file's data with linking information included.
 */
class LinkedObjectFile {
 public:
  LinkedObjectFile() = default;
  void set_segment_count(int n_segs);
  void push_back_word_to_segment(uint32_t word, int segment);
  int get_label_id_for(int seg, int offset);
  int get_label_at(int seg, int offset) const;
  bool label_points_to_code(int label_id) const;
  bool pointer_link_word(int source_segment, int source_offset, int dest_segment, int dest_offset);
  void pointer_link_split_word(int source_segment,
                               int source_hi_offset,
                               int source_lo_offset,
                               int dest_segment,
                               int dest_offset);
  void symbol_link_word(int source_segment,
                        int source_offset,
                        const char* name,
                        LinkedWord::Kind kind);
  void symbol_link_offset(int source_segment, int source_offset, const char* name);
  Function& get_function_at_label(int label_id);
  std::string get_label_name(int label_id) const;
  uint32_t set_ordered_label_names();
  void find_code();
  std::string print_words();
  void find_functions();
  void disassemble_functions();
  void process_fp_relative_links();
  std::string print_scripts();
  std::string print_disassembly();
  bool has_any_functions();
  void append_word_to_string(std::string& dest, const LinkedWord& word) const;

  struct Stats {
    uint32_t total_code_bytes = 0;
    uint32_t total_v2_code_bytes = 0;
    uint32_t total_v2_pointers = 0;
    uint32_t total_v2_pointer_seeks = 0;
    uint32_t total_v2_link_bytes = 0;
    uint32_t total_v2_symbol_links = 0;
    uint32_t total_v2_symbol_count = 0;

    uint32_t v3_code_bytes = 0;
    uint32_t v3_pointers = 0;
    uint32_t v3_split_pointers = 0;
    uint32_t v3_word_pointers = 0;
    uint32_t v3_pointer_seeks = 0;
    uint32_t v3_link_bytes = 0;

    uint32_t v3_symbol_count = 0;
    uint32_t v3_symbol_link_offset = 0;
    uint32_t v3_symbol_link_word = 0;

    uint32_t data_bytes = 0;
    uint32_t code_bytes = 0;

    uint32_t function_count = 0;
    uint32_t decoded_ops = 0;

    uint32_t n_fp_reg_use = 0;
    uint32_t n_fp_reg_use_resolved = 0;

    void add(const Stats& other) {
      total_code_bytes += other.total_code_bytes;
      total_v2_code_bytes += other.total_v2_code_bytes;
      total_v2_pointers += other.total_v2_pointers;
      total_v2_pointer_seeks += other.total_v2_pointer_seeks;
      total_v2_link_bytes += other.total_v2_link_bytes;
      total_v2_symbol_links += other.total_v2_symbol_links;
      total_v2_symbol_count += other.total_v2_symbol_count;
      v3_code_bytes += other.v3_code_bytes;
      v3_pointers += other.v3_pointers;
      v3_pointer_seeks += other.v3_pointer_seeks;
      v3_link_bytes += other.v3_link_bytes;
      v3_word_pointers += other.v3_word_pointers;
      v3_split_pointers += other.v3_split_pointers;
      v3_symbol_count += other.v3_symbol_count;
      v3_symbol_link_offset += other.v3_symbol_link_offset;
      v3_symbol_link_word += other.v3_symbol_link_word;
      data_bytes += other.data_bytes;
      code_bytes += other.code_bytes;
      function_count += other.function_count;
      decoded_ops += other.decoded_ops;
      n_fp_reg_use += other.n_fp_reg_use;
      n_fp_reg_use_resolved += other.n_fp_reg_use_resolved;
    }
  } stats;

  int segments = 0;
  std::vector<std::vector<LinkedWord>> words_by_seg;
  std::vector<uint32_t> offset_of_data_zone_by_seg;
  std::vector<std::vector<Function>> functions_by_seg;
  std::vector<Label> labels;

 private:
  std::shared_ptr<Form> to_form_script(int seg, int word_idx, std::vector<bool>& seen);
  std::shared_ptr<Form> to_form_script_object(int seg, int byte_idx, std::vector<bool>& seen);
  bool is_empty_list(int seg, int byte_idx);
  bool is_string(int seg, int byte_idx);
  std::string get_goal_string(int seg, int word_idx);

  std::vector<std::unordered_map<int, int>> label_per_seg_by_offset;
};

#endif  // NEXT_LINKEDOBJECTFILE_H
