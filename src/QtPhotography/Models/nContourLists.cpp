#include <qtphotography.h>

N::ContourLists:: ContourLists (QWidget * parent,Plan * p)
                : ListDock     (          parent,       p)
{
  WidgetClass   ;
  Configure ( ) ;
}

N::ContourLists::~ContourLists(void)
{
}

QSize N::ContourLists::sizeHint(void) const
{
  return SizeSuggestion ( QSize ( 640 , 480 ) ) ;
}

void N::ContourLists::Configure(void)
{
  setAccessibleName            ( "N::ContourLists"     ) ;
  setObjectName                ( "N::ContourLists"     ) ;
  setWindowTitle               ( tr("Contour lists")   ) ;
  setViewMode                  ( IconMode              ) ;
  setIconSize                  ( QSize ( 128 , 128 )   ) ;
  setGridSize                  ( QSize ( 144 , 192 )   ) ;
  setMovement                  ( Snap                  ) ;
  setResizeMode                ( Adjust                ) ;
  setSelectionMode             ( SingleSelection       ) ;
  setWordWrap                  ( true                  ) ;
  setMouseTracking             ( true                  ) ;
  setWrapping                  ( true                  ) ;
  setTextElideMode             ( Qt::ElideNone         ) ;
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAsNeeded ) ;
  setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded ) ;
  setMinimumSize               ( QSize ( 128 , 200 )   ) ;
  setDragDropMode              ( DragOnly              ) ;
  plan -> setFont              ( this                  ) ;
}

bool N::ContourLists::FocusIn(void)
{
  nKickOut          ( IsNull(plan) , false           ) ;
  DisableAllActions (                                ) ;
  AssignAction      ( Label        , windowTitle ( ) ) ;
  LinkAction        ( Refresh      , startup     ( ) ) ;
  LinkAction        ( Copy         , Copy        ( ) ) ;
  return true                                          ;
}

QMimeData * N::ContourLists::dragMime(void)
{
  QListWidgetItem * IT = currentItem()            ;
  if (IsNull(IT)) return NULL                     ;
  SUID  uuid  = nListUuid(IT)                     ;
  QMimeData * mime = new QMimeData()              ;
  setMime ( mime , "contour/uuid"  , uuid )       ;
  if (NotNull(IT))                                {
    QIcon icon  = IT->icon()                      ;
    if (!icon.isNull())                           {
      QSize  s     = iconSize()                   ;
      QImage image = icon . pixmap(s) . toImage() ;
      if (!image.isNull())                        {
        mime -> setImageData(image)               ;
      }                                           ;
    }                                             ;
  }                                               ;
  return mime                                     ;
}

bool N::ContourLists::hasItem(void)
{
  QListWidgetItem * item = currentItem() ;
  return NotNull ( item )                ;
}

bool N::ContourLists::startDrag(QMouseEvent * event)
{
  QListWidgetItem * atItem = itemAt(event->pos())            ;
  if (IsNull(atItem)) return false                           ;
  if (!IsMask(event->buttons(),Qt::LeftButton)) return false ;
  dragPoint = event->pos()                                   ;
  if (!atItem->isSelected()) return false                    ;
  if (!PassDragDrop) return true                             ;
  return false                                               ;
}

bool N::ContourLists::fetchDrag(QMouseEvent * event)
{
  if (!IsMask(event->buttons(),Qt::LeftButton)) return false   ;
  QPoint pos = event->pos()                                    ;
  pos -= dragPoint                                             ;
  return ( pos.manhattanLength() > qApp->startDragDistance() ) ;
}

void N::ContourLists::dragDone(Qt::DropAction dropIt,QMimeData * mime)
{
}

bool N::ContourLists::finishDrag(QMouseEvent * event)
{
  return true ;
}

void N::ContourLists::run(int T,ThreadData * d)
{
  nDropOut         ( ! IsContinue ( d ) ) ;
  switch           ( T                  ) {
    case 10001                            :
      startLoading (                    ) ;
      Load         ( d                  ) ;
      stopLoading  (                    ) ;
    break                                 ;
  }                                       ;
}

bool N::ContourLists::Load(ThreadData * d)
{
  WaitClear     (                                  ) ;
  Bustle        (                                  ) ;
  ConnectSQL    ( SC , plan -> sql                 ) ;
    QString Q                                        ;
    QString name                                     ;
    SUID    uuid                                     ;
    UUIDs   Uuids                                    ;
    Uuids = SC . Uuids                               (
      PlanTable(Contours)                            ,
      "uuid"                                         ,
      SC . OrderByAsc ( "id" )                     ) ;
    foreach ( uuid , Uuids )                         {
      QIcon icon                                     ;
      name = SC . getName                            (
        PlanTable(Names)                             ,
        "uuid"                                       ,
        vLanguageId                                  ,
        uuid                                       ) ;
      Q = SC . sql . SelectFrom                      (
            "snap"                                   ,
            PlanTable(Contours)                      ,
            SC . WhereUuid ( uuid )                ) ;
      if ( SC . Fetch ( Q ) )                        {
        QByteArray snap = SC . ByteArray ( 0 )       ;
        QByteArray Snap = qUncompress(snap)          ;
        QImage *   I = new QImage()                  ;
        if ( I -> loadFromData ( Snap , "PNG" ) )    {
          icon = QIcon(QPixmap::fromImage(*I))       ;
        }                                            ;
        snap . clear ( )                             ;
        Snap . clear ( )                             ;
        delete I                                     ;
      }                                              ;
      addItem   ( uuid , icon , name , name        ) ;
    }                                                ;
  DisconnectSQL ( SC                               ) ;
  Vacancy       (                                  ) ;
  alert         ( "Done" , tr("Contours")          ) ;
  ////////////////////////////////////////////////////
  return true                                        ;
}

bool N::ContourLists::startup(void)
{
  start ( 10001 ) ;
  return true     ;
}

void N::ContourLists::List(void)
{
  startup ( ) ;
}

void N::ContourLists::Copy(void)
{
  QMimeData * mime = dragMime    (      ) ;
  if (IsNull(mime)) return                ;
  qApp->clipboard()->setMimeData ( mime ) ;
}

bool N::ContourLists::Menu(QPoint pos)
{
  MenuManager mm (this)                     ;
  QAction   * aa                            ;
  mm . add        ( 102 , tr("Refresh") )   ;
  DockingMenu     ( mm      )               ;
  mm . setFont    ( plan    )               ;
  aa = mm . exec  (         )               ;
  if (IsNull      (      aa ) ) return true ;
  if ( RunDocking ( mm , aa ) ) return true ;
  switch ( mm [ aa ] )                      {
    case 102                                :
      startup ( )                           ;
    break                                   ;
    default                                 :
    break                                   ;
  }                                         ;
  return true                               ;
}
