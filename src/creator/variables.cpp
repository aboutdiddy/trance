#include "variables.h"
#include "item_list.h"
#include "main.h"
#include "../common.h"
#include "../session.h"

#pragma warning(push, 0)
#include <src/trance.pb.h>
#include <wx/sizer.h>
#include <wx/splitter.h>
#pragma warning(pop)

VariablePage::VariablePage(wxNotebook* parent,
                           CreatorFrame& creator_frame,
                           trance_pb::Session& session)
: wxNotebookPage{parent, wxID_ANY}
, _creator_frame{creator_frame}
, _session(session)
{
  auto sizer = new wxBoxSizer{wxVERTICAL};
  auto splitter = new wxSplitterWindow{
      this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
      wxSP_THIN_SASH | wxSP_LIVE_UPDATE};
  splitter->SetSashGravity(0);
  splitter->SetMinimumPaneSize(128);

  auto bottom_panel = new wxPanel{splitter, wxID_ANY};
  auto bottom = new wxBoxSizer{wxHORIZONTAL};

  _item_list = new ItemList<trance_pb::Variable>{
      splitter, *session.mutable_variable_map(), "variable",
      [&](const std::string& s) { _item_selected = s; RefreshOurData(); },
      [](const std::string&) {}, [](const std::string&) {},
      [](const std::string&, const std::string&) {}, true};

  bottom_panel->SetSizer(bottom);
  
  sizer->Add(splitter, 1, wxEXPAND, 0);
  splitter->SplitHorizontally(_item_list, bottom_panel);
  SetSizer(sizer);
}

VariablePage::~VariablePage()
{
}

void VariablePage::RefreshOurData()
{
}

void VariablePage::RefreshData()
{
  RefreshOurData();
}