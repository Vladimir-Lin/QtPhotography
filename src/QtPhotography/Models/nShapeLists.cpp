#include <qtphotography.h>

#pragma message("ShapeLists is required to be completed")

N::ShapeLists:: ShapeLists      (QWidget * parent,Plan * p)
              : ItemEditor      (          parent,       p)
              , GraphicsManager (                        p)
{
  WidgetClass   ;
  Configure ( ) ;
}

N::ShapeLists::~ShapeLists (void)
{
}

void N::ShapeLists::Configure(void)
{
  PassDragDrop = false                          ;
  ItemTable    = PlanTable(Shapes)              ;
  ItemType     = Types::Shapes                  ;
  ///////////////////////////////////////////////
  setDropFlag ( DropText     , false )          ;
  setDropFlag ( DropUrls     , false )          ;
  setDropFlag ( DropImage    , false )          ;
  setDropFlag ( DropHtml     , false )          ;
  setDropFlag ( DropColor    , false )          ;
  setDropFlag ( DropTag      , false )          ;
  setDropFlag ( DropPicture  , false )          ;
  setDropFlag ( DropPeople   , false )          ;
  setDropFlag ( DropVideo    , false )          ;
  setDropFlag ( DropAlbum    , false )          ;
  setDropFlag ( DropGender   , false )          ;
  setDropFlag ( DropDivision , false )          ;
  setDropFlag ( DropURIs     , false )          ;
  setDropFlag ( DropBookmark , false )          ;
  setDropFlag ( DropFont     , false )          ;
  setDropFlag ( DropPen      , false )          ;
  setDropFlag ( DropBrush    , false )          ;
  setDropFlag ( DropGradient , false )          ;
  ///////////////////////////////////////////////
  setAccessibleName       ( "N::ShapeLists"   ) ;
  setObjectName           ( "N::ShapeLists"   ) ;
  setFocusPolicy          ( Qt::WheelFocus    ) ;
  setDragDropMode         ( DragDrop          ) ;
  setAlternatingRowColors ( true              ) ;
  setRootIsDecorated      ( false             ) ;
  setSelectionMode        ( ExtendedSelection ) ;
  setColumnCount          ( 1                 ) ;
  ///////////////////////////////////////////////
  QStringList headers                           ;
  headers << tr("Shape")                        ;
  setHeaderLabels (headers)                     ;
  setWindowTitle  (tr("Shape manager")      )   ;
  ///////////////////////////////////////////////
  MountClicked    ( 2                       )   ;
  plan -> setFont ( this                    )   ;
}

bool N::ShapeLists::FocusIn(void)
{
  nKickOut          ( IsNull(plan) , true            ) ;
  DisableAllActions (                                ) ;
  AssignAction      ( Label        , windowTitle ( ) ) ;
  LinkAction        ( Insert       , Insert      ( ) ) ;
  LinkAction        ( Delete       , Delete      ( ) ) ;
  LinkAction        ( Copy         , Copy        ( ) ) ;
  return true                                          ;
}

void N::ShapeLists::Insert(void)
{
}

void N::ShapeLists::Delete(void)
{
}

void N::ShapeLists::Copy(void)
{
}
