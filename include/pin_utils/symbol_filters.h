#pragma once
#include <pin.H>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <fstream>

#include "filter.h"

namespace pene
{
  namespace pin_utils
  {
    class symbol_filter_base : public virtual filter
    {
    public:
      symbol_filter_base() = delete;

    protected:
      symbol_filter_base(const std::string& sym_lis_filename);
      BOOL is_in_list(ADDRINT) const;

    private:
      using name_list_t = std::tr1::unordered_set<std::string>;
      using name_list_by_obj_name_t = std::tr1::unordered_map<std::string, name_list_t>;
      // sym_exlude_list[obj_name] => func_name_list
      name_list_by_obj_name_t sym_list;
    };

    class symbol_exclude_filter final : public virtual symbol_filter_base
    {
    public:
      symbol_exclude_filter(const std::string& sym_lis_filename);
      virtual BOOL is_instrumented(ADDRINT) const override;
    };

    class symbol_include_filter final : public virtual symbol_filter_base
    {
    public:
      symbol_include_filter(const std::string& sym_lis_filename);
      virtual BOOL is_instrumented(ADDRINT) const override;
    };




  }
}