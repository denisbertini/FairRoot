#ifndef FAIRDBWTTREEVIEW_H
#define FAIRDBWTTREEVIEW_H

#include <fstream>

#include <WTree>
#include <WtreeNode>
#include <WMenuItem>
#include <WTabWidget>
#include <WTextArea>

#include "FairDbWtFileModel.h"
#include "FairDbWtFileEditDialog.h"


#include <Chart/WPieChart>

#include "CsvUtil.h"

using namespace Wt;

 
class FileTreeTable;

class FairDbWtTreeView : public WContainerWidget
{

public:
  FairDbWtTreeView(WContainerWidget *parent = 0);
  virtual ~FairDbWtTreeView();



  WText* createTitle(const WString& title);

  FileTreeTable* TreeTableView();
  void TreeTableChanged();

  WTreeView* folderView();
  void folderChanged();
  void showPopup(const WModelIndex& item, const WMouseEvent& event);
  void showMeMouse(const WMouseEvent& event);

  void popupAction(); 
  void dialogDone();


  //WTableView *fileView();
  //void editFile(const WModelIndex& item); 
  //WWidget *pieChart();  
  //WWidget *aboutDisplay();  
  //void populateFiles();
  //void convertToDate(WStandardItem *item);
  //void convertToNumber(WStandardItem *item);

  void populateFolders();
 
  WStandardItem *createFolderItem(const WString& location,
                  const std::string& folderId);


  

private:


  WStandardItemModel    *fFolderModel;
  WStandardItemModel    *fFileModel;
  WSortFilterProxyModel *fFileFilterModel;

  std::map<std::string, WString> fFolderNameMap;

  FileTreeTable* fFileTreeTable;
  WTabWidget* fTabWidget;
  
  WTreeView *fFolderView;
  WTableView *fFileView;
  WPopupMenu *fPopup;
  WMessageBox *fPopupActionBox;

  typedef std::set<WTreeNode *> WTreeNodeSet;
  
 // Events Handling
  Wt::WWidget *wMouseEvent();
  void showClicked(const Wt::WMouseEvent &e);
  void showDoubleClicked(const Wt::WMouseEvent &e);
  void showMouseWentOut(const Wt::WMouseEvent &e);
  void showMouseWentOver(const Wt::WMouseEvent &e);
  void showMouseMoved(const Wt::WMouseEvent &e);
  void showMouseWentUp(const Wt::WMouseEvent &e);
  void showMouseWentDown(const Wt::WMouseEvent &e);
  void showMouseWheel(const Wt::WMouseEvent &e);

  Wt::WText *mouseEventType_;
  Wt::WText *mouseEventDescription_;

  void describe(const Wt::WMouseEvent &e);



};

#endif //FAIRDBWTTREEVIEW_H
