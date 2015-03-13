#include <WText>
#include <WMenu>
#include <WStackedWidget>
#include <WVBoxLayout>
#include <WHBoxLayout>
#include <WNavigationBar>
#include <Wt/WTreeTableNode>
#include <WPanel>
#include <WTable>

#include "FileTreeTable.h"
#include "FairDbWtTreeView.h"

#include <iostream>
using namespace std;

using namespace Wt;

FairDbWtTreeView::FairDbWtTreeView(WContainerWidget *parent):
  WContainerWidget(parent)
{
 
     setStyleClass("maindiv");

     WGridLayout *layout = new WGridLayout(this);
     layout->addWidget(createTitle("Parameters"), 0, 0);
     layout->addWidget(createTitle("View"), 0, 1);
     layout->addWidget(TreeTableView(), 1, 0); 
     //layout->setColumnResizable(0);

    
     // select the first folder
     //fFolderView->select(fFolderModel->index(0, 0, fFolderModel->index(0, 0)));

    // DB Comment for Now !!

    
     WVBoxLayout *vbox = new WVBoxLayout();
     //vbox->addWidget(fileView(), 1);
     //vbox->addWidget(pieChart(), 1);
     //vbox->setResizable(0);
     //vbox->addWidget(fileView(), 1);
        
     fTabWidget = new WTabWidget(); 
     fTabWidget->setStyleClass("tabwidget");  
     vbox->addWidget(fTabWidget, 1);

     layout->addLayout(vbox, 1, 1);
     //layout->addWidget(aboutDisplay(), 2, 0, 1, 2);
     //layout->setRowStretch(1, 1);
     //layout->setColumnStretch(1, 1);
     
   
    
     setLayout(layout);

}

FairDbWtTreeView::~FairDbWtTreeView(){

    if (fPopup) delete fPopup;
    if (fPopupActionBox) delete fPopupActionBox;
}

WText* FairDbWtTreeView::createTitle(const WString& title) {
    WText *result = new WText(title);
    result->setInline(false);
    result->setStyleClass("title");
    return result;
}

FileTreeTable* FairDbWtTreeView::TreeTableView(){

     FileTreeTable *treeTable = new FileTreeTable(".");
     //treeTable->resize(500, 300);
     treeTable->resize(500, WLength::Auto);
     treeTable->tree()->setSelectionMode(ExtendedSelection);
     treeTable->treeRoot()->setNodeVisible(false);
     treeTable->treeRoot()->setChildCountPolicy(WTreeNode::Enabled);

     treeTable->tree()->itemSelectionChanged()
                    .connect(this, &FairDbWtTreeView::TreeTableChanged); 
  


    fFileTreeTable=treeTable; 
    return treeTable; 
}

void FairDbWtTreeView::TreeTableChanged() {
WTree* aTree = fFileTreeTable->tree(); 
for (WTreeNodeSet::const_iterator i = aTree->selectedNodes().begin(); 
                                  i != aTree->selectedNodes().end(); ++i){
    WTreeNode *aNode=(*i);

cout << "-I Server: FairDbWtTreeView::TreeTableChanged() Node:" <<  aNode->label()->text().value() << endl;     

     Wt::WTable *table = new Wt::WTable(this);
     table->elementAt(0, 0)->addWidget(new Wt::WText("Item @ row 0, column 0"));
     table->elementAt(0, 1)->addWidget(new Wt::WText("Item @ row 0, column 1"));
     table->elementAt(1, 0)->addWidget(new Wt::WText("Item @ row 1, column 0"));
     table->elementAt(1, 1)->addWidget(new Wt::WText("Item @ row 1, column 1"));
     Wt::WTableCell *cell = table->elementAt(2, 0);
     cell->addWidget(new Wt::WText("Item @ row 2"));
     cell->setColumnSpan(2);

   fTabWidget->addTab(table, "FirstPar", WTabWidget::PreLoading);

}

}

WTreeView* FairDbWtTreeView::folderView() {
    WTreeView *treeView = new FolderView();
    treeView->setRowHeight(25);   
    treeView->setAttributeValue
      ("oncontextmenu",
       "event.cancelBubble = true; event.returnValue = false; return false;");
     treeView->setModel(fFolderModel);
     treeView->resize(200, WLength::Auto);
     treeView->setSelectionMode(SingleSelection);
     treeView->expandToDepth(1);
     treeView->selectionChanged()
       .connect(this, &FairDbWtTreeView::folderChanged);

     treeView->mouseWentUp().connect(this, &FairDbWtTreeView::showPopup);

    fFolderView = treeView;

    return treeView;
  }


void FairDbWtTreeView::folderChanged() {
    if (fFolderView->selectedIndexes().empty())
      return;

    WModelIndex selected = *fFolderView->selectedIndexes().begin();
    boost::any d = selected.data(UserRole);
    if (!d.empty()) {
      std::string folder = boost::any_cast<std::string>(d);

      // For simplicity, we assume here that the folder-id does not
      // contain special regexp characters, otherwise these need to be
      // escaped -- or use the \Q \E qutoing escape regular expression
      // syntax (and escape \E)
      fFileFilterModel->setFilterRegExp(folder);
    }
  }


void FairDbWtTreeView::showPopup(const WModelIndex& item, const WMouseEvent& event) {
    if (event.button() == WMouseEvent::RightButton) {
      // Select the item, it was not yet selected.
      if (!fFolderView->isSelected(item))
    fFolderView->select(item);

      if (!fPopup) {
    fPopup = new WPopupMenu();
    fPopup->addItem("icons/folder_new.gif", "Create a New Folder");
    fPopup->addItem("Rename this Folder")->setCheckable(true);
    fPopup->addItem("Delete this Folder");
    fPopup->addSeparator();
    fPopup->addItem("Folder Details");
    fPopup->addSeparator();
    fPopup->addItem("Application Inventory");
    fPopup->addItem("Hardware Inventory");
    fPopup->addSeparator();

    WPopupMenu *subMenu = new WPopupMenu();
    subMenu->addItem("Sub Item 1");
    subMenu->addItem("Sub Item 2");
    fPopup->addMenu("File Deployments", subMenu);

    /*
     * This is one method of executing a popup, which does not block a
     * thread for a reentrant event loop, and thus scales.
     *
     * Alternatively you could call WPopupMenu::exec(), which returns
     * the result, but while waiting for it, blocks the thread.
     */      
    fPopup->aboutToHide().connect(this, &FairDbWtTreeView::popupAction);
      }

      if (fPopup->isHidden())
         fPopup->popup(event);
      else
         fPopup->hide();
     }//! if (event) 
  }


void FairDbWtTreeView::popupAction() {
    if (fPopup->result()) {
      /*
       * You could also bind extra data to an item using setData() and
       * check here for the action asked. For now, we just use the text.
       */
      WString text = fPopup->result()->text();
      fPopup->hide();

      fPopupActionBox = new WMessageBox("Sorry.","Action '" + text
                    + "' is not implemented.", NoIcon, Ok);
      fPopupActionBox->buttonClicked().connect(this, &FairDbWtTreeView::dialogDone);
      fPopupActionBox->show();
    } else {
      fPopup->hide();
    }
  }


void FairDbWtTreeView::dialogDone() {
    if (fPopupActionBox) {
        delete fPopupActionBox;
        fPopupActionBox = 0;
    }
}



WStandardItem * FairDbWtTreeView::createFolderItem(const WString& location,
                  const std::string& folderId = std::string()){

    WStandardItem *result = new WStandardItem(location);

    if (!folderId.empty()) {
      result->setData(boost::any(folderId));
      result->setFlags(result->flags() | ItemIsDropEnabled);
      fFolderNameMap[folderId] = location;
    } else
      result->setFlags(result->flags().clear(ItemIsSelectable));

    result->setIcon("icons/folder.gif");

    return result;
}







void FairDbWtTreeView::populateFolders() {
    WStandardItem *level1, *level2;

    fFolderModel->appendRow(level1 = createFolderItem("San Fransisco"));
    level1->appendRow(level2 = createFolderItem("Investors", "sf-investors"));
    level1->appendRow(level2 = createFolderItem("Fellows", "sf-fellows"));

    fFolderModel->appendRow(level1 = createFolderItem("Sophia Antipolis"));
    level1->appendRow(level2 = createFolderItem("R&D", "sa-r_d"));
    level1->appendRow(level2 = createFolderItem("Services", "sa-services"));
    level1->appendRow(level2 = createFolderItem("Support", "sa-support"));
    level1->appendRow(level2 = createFolderItem("Billing", "sa-billing"));

    fFolderModel->appendRow(level1 = createFolderItem("New York"));
    level1->appendRow(level2 = createFolderItem("Marketing", "ny-marketing"));
    level1->appendRow(level2 = createFolderItem("Sales", "ny-sales"));
    level1->appendRow(level2 = createFolderItem("Advisors", "ny-advisors"));

    fFolderModel->appendRow(level1 = createFolderItem
                 (WString::fromUTF8("Frankfürt")));
    level1->appendRow(level2 = createFolderItem("Sales", "frank-sales"));

    fFolderModel->setHeaderData(0, Horizontal,
                 boost::any(std::string("SandBox")));
}












