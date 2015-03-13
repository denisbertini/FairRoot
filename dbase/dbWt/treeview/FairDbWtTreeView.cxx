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

// Events Handling
#include <Wt/WBreak>
#include <Wt/WCssDecorationStyle>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <Wt/WLineEdit>
#include <Wt/WHBoxLayout>
#include <Wt/Utils>


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

    
     WVBoxLayout *vbox = new WVBoxLayout(this);
     //vbox->addWidget(fileView(), 1);
     //vbox->addWidget(pieChart(), 1);
     vbox->setResizable(0);
     //vbox->addWidget(fileView(), 1);
     //WContainerWidget *rFrame = new WContainerWidget(this);   
     //rFrame->mouseWentUp().connect(this, &FairDbWtTreeView::showMeMouse);
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


void FairDbWtTreeView::showMeMouse(const WMouseEvent& event) {
 if (event.button() == WMouseEvent::RightButton) {
    cout << "-I- RIGHT BUTTON FROM MOUSE" << endl;
}
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

     WMenuItem *tab = fTabWidget->addTab(table, "FirstPar", WTabWidget::PreLoading);
     tab->setCloseable(true);

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


//Event Handling

WWidget *FairDbWtTreeView::wMouseEvent()
{
  WContainerWidget *result = new WContainerWidget();

  //topic("WMouseEvent", result);
  //addText(tr("events-WMouseEvent"), result);
  WContainerWidget *c = new WContainerWidget(result);
  WHBoxLayout *hlayout = new WHBoxLayout;
  c->setLayout(hlayout);
  WContainerWidget *l = new WContainerWidget;
  WContainerWidget *r = new WContainerWidget;
  new WText("clicked<br/>doubleClicked<br/>mouseWentOut<br/>mouseWentOver",
        l);
  new WText("mouseWentDown<br/>mouseWentUp<br/>mouseMoved<br/>mouseWheel", r);
  hlayout->addWidget(l);
  hlayout->addWidget(r);
  c->resize(600, 300);
  l->decorationStyle().setBackgroundColor(Wt::gray);
  r->decorationStyle().setBackgroundColor(Wt::gray);
  // prevent that firefox interprets drag as drag&drop action
  l->setStyleClass("unselectable");
  r->setStyleClass("unselectable");
  l->clicked().connect(this, &FairDbWtTreeView::showClicked);
  l->doubleClicked().connect(this, &FairDbWtTreeView::showDoubleClicked);
  l->mouseWentOut().connect(this, &FairDbWtTreeView::showMouseWentOut);
  l->mouseWentOver().connect(this, &FairDbWtTreeView::showMouseWentOver);
  r->mouseMoved().connect(this, &FairDbWtTreeView::showMouseMoved);
  r->mouseWentUp().connect(this, &FairDbWtTreeView::showMouseWentUp);
  r->mouseWentDown().connect(this, &FairDbWtTreeView::showMouseWentDown);
  r->mouseWheel().connect(this, &FairDbWtTreeView::showMouseWheel);
  r->mouseWheel().preventDefaultAction(true);

  l->setAttributeValue
    ("oncontextmenu",
     "event.cancelBubble = true; event.returnValue = false; return false;");
  r->setAttributeValue
    ("oncontextmenu",
     "event.cancelBubble = true; event.returnValue = false; return false;");

  new WBreak(result);
  new WText("Last event: ", result);
  mouseEventType_ = new WText(result);
  new WBreak(result);
  mouseEventDescription_ = new WText(result);

  return result;
}



void FairDbWtTreeView::showClicked(const WMouseEvent &e)
{
  mouseEventType_->setText("clicked");
  describe(e);
}

void FairDbWtTreeView::showDoubleClicked(const WMouseEvent &e)
{
  mouseEventType_->setText("doubleClicked");
  describe(e);
}

void FairDbWtTreeView::showMouseWentOut(const WMouseEvent &e)
{
  mouseEventType_->setText("mouseWentOut");
  describe(e);
}

void FairDbWtTreeView::showMouseWheel(const WMouseEvent &e)
{
  mouseEventType_->setText("mouseWheel");
  describe(e);
}

void FairDbWtTreeView::showMouseWentOver(const WMouseEvent &e)
{
  mouseEventType_->setText("mouseWentOver");
  describe(e);
}

void FairDbWtTreeView::showMouseMoved(const WMouseEvent &e)
{
  mouseEventType_->setText("mouseMoved");
  describe(e);
}

void FairDbWtTreeView::showMouseWentUp(const WMouseEvent &e)
{
  mouseEventType_->setText("mouseWentUp");
  describe(e);
}

void FairDbWtTreeView::showMouseWentDown(const WMouseEvent &e)
{
  mouseEventType_->setText("mouseWentDown");
  describe(e);
}


namespace  my_strutils{
  std::ostream &operator<<(std::ostream &o, Wt::WMouseEvent::Button b)
  {
    switch (b) {
    case WMouseEvent::NoButton:
      return o << "No button";
    case WMouseEvent::LeftButton:
      return o << "LeftButton";
    case WMouseEvent::RightButton:
      return o << "RightButton";
    case WMouseEvent::MiddleButton:
      return o << "MiddleButton";
    default:
      return o << "Unknown Button";
    }
  }

  std::ostream &operator<<(std::ostream &o, Wt::Key k)
  {
    switch(k) {
    default:
    case Key_unknown : return o << "Key_unknown";
    case Key_Enter : return o << "Key_Enter";
    case Key_Tab : return o << "Key_Tab";
    case Key_Backspace : return o << "Key_Backspace";
    case Key_Shift : return o << "Key_Shift";
    case Key_Control : return o << "Key_Control";
    case Key_Alt : return o << "Key_Alt";
    case Key_PageUp : return o << "Key_PageUp";
    case Key_PageDown : return o << "Key_PageDown";
    case Key_End : return o << "Key_End";
    case Key_Home : return o << "Key_Home";
    case Key_Left : return o << "Key_Left";
    case Key_Up : return o << "Key_Up";
    case Key_Right : return o << "Key_Right";
    case Key_Down : return o << "Key_Down";
    case Key_Insert : return o << "Key_Insert";
    case Key_Delete : return o << "Key_Delete";
    case Key_Escape : return o << "Key_Escape";
    case Key_F1 : return o << "Key_F1";
    case Key_F2 : return o << "Key_F2";
    case Key_F3 : return o << "Key_F3";
    case Key_F4 : return o << "Key_F4";
    case Key_F5 : return o << "Key_F5";
    case Key_F6 : return o << "Key_F6";
    case Key_F7 : return o << "Key_F7";
    case Key_F8 : return o << "Key_F8";
    case Key_F9 : return o << "Key_F9";
    case Key_F10 : return o << "Key_F10";
    case Key_F11 : return o << "Key_F11";
    case Key_F12 : return o << "Key_F12";
    case Key_Space : return o << "Key_Space";
    case Key_A : return o << "Key_A";
    case Key_B : return o << "Key_B";
    case Key_C : return o << "Key_C";
    case Key_D : return o << "Key_D";
    case Key_E : return o << "Key_E";
    case Key_F : return o << "Key_F";
    case Key_G : return o << "Key_G";
    case Key_H : return o << "Key_H";
    case Key_I : return o << "Key_I";
    case Key_J : return o << "Key_J";
    case Key_K : return o << "Key_K";
    case Key_L : return o << "Key_L";
    case Key_M : return o << "Key_M";
    case Key_N : return o << "Key_N";
    case Key_O : return o << "Key_O";
    case Key_P : return o << "Key_P";
    case Key_Q : return o << "Key_Q";
    case Key_R : return o << "Key_R";
    case Key_S : return o << "Key_S";
    case Key_T : return o << "Key_T";
    case Key_U : return o << "Key_U";
    case Key_V : return o << "Key_V";
    case Key_W : return o << "Key_W";
    case Key_X : return o << "Key_X";
    case Key_Y : return o << "Key_Y";
    case Key_Z : return o << "Key_Z";
    }
  }

  std::ostream &operator<<(std::ostream &o, Wt::WMouseEvent::Coordinates c)
  {
    return o << c.x << ", " << c.y;
  }
  std::string modifiersToString(const WFlags< KeyboardModifier >& modifiers)
  {
    std::stringstream o;
    if (modifiers & ShiftModifier) o << "Shift ";
    if (modifiers & ControlModifier) o << "Control ";
    if (modifiers & AltModifier) o << "Alt ";
    if (modifiers & MetaModifier) o << "Meta ";
    if (modifiers == 0) o << "No modifiers";
    return o.str();
  }
}



void FairDbWtTreeView::describe(const Wt::WMouseEvent &e)
{
  std::stringstream ss;
  ss << "Button: " << e.button() << "<br/>"
     << "Modifiers: " << my_strutils::modifiersToString(e.modifiers()) << "<br/>";
     //<< "Document coordinates: " << my_strutils::modifiersToString(e.document()) << "<br/>"
     //<< "Window coordinates: " << e.window() << "<br/>"
     //<< "Screen coordinates: " << e.screen() << "<br/>"
     //<< "Widget coordinates: " << e.widget() << "<br/>"
     //<< "DragDelta coordinates: " << e.dragDelta() << "<br/>"
     //<< "Wheel delta: " << e.wheelDelta() << "<br/>";
  mouseEventDescription_->setText(ss.str());
}







