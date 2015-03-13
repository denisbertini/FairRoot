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
   
};

#endif //FAIRDBWTTREEVIEW_H
