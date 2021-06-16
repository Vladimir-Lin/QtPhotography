#include <qtphotography.h>

N::ColorGroups:: ColorGroups ( QWidget * parent , Plan * p )
               : TreeDock    (           parent ,        p )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::ColorGroups::~ColorGroups(void)
{
}

void N::ColorGroups::Configure(void)
{
  NewTreeWidgetItem       ( head                                  ) ;
  ///////////////////////////////////////////////////////////////////
  setAccessibleName       ( "nColorGroups"                        ) ;
  setWindowTitle          ( tr("Color groups")                    ) ;
  setWindowIcon           ( plan->Icon(Types::ColorGroup,1,0)     ) ;
  ///////////////////////////////////////////////////////////////////
  setColumnCount          ( 2                                     ) ;
  setRootIsDecorated      ( false                                 ) ;
  setAlternatingRowColors ( true                                  ) ;
  setSelectionMode        ( ExtendedSelection                     ) ;
  ///////////////////////////////////////////////////////////////////
  head -> setText         ( 0 , tr("Name")                        ) ;
  head -> setText         ( 1 , tr("Colors")                      ) ;
  setAlignment            ( 1 , Qt::AlignRight | Qt::AlignVCenter ) ;
  assignHeaderItems       ( head                                  ) ;
  ///////////////////////////////////////////////////////////////////
  plan -> setFont         ( this                                  ) ;
  setDragDropMode         ( DragDrop                              ) ;
  ///////////////////////////////////////////////////////////////////
  dockingOrientation = Qt::Vertical                                 ;
  dockingPlace       = Qt::RightDockWidgetArea                      ;
  dockingPlaces      = Qt::LeftDockWidgetArea                       |
                       Qt::RightDockWidgetArea                      ;
  ///////////////////////////////////////////////////////////////////
  MountClicked            ( 1                                     ) ;
  MountClicked            ( 2                                     ) ;
}

bool N::ColorGroups::FocusIn(void)
{
  AssignAction ( Label   , windowTitle ( ) ) ;
  LinkAction   ( Insert  , Insert      ( ) ) ;
  LinkAction   ( Refresh , startup     ( ) ) ;
  return true                                ;
}

bool N::ColorGroups::hasItem(void)
{
  UUIDs Uuids = selectedUuids ( 0 ) ;
  return ( Uuids . count ( ) > 0  ) ;
}

bool N::ColorGroups::startDrag(QMouseEvent * event)
{
  QTreeWidgetItem * atItem = itemAt(event->pos())               ;
  nKickOut (  IsNull(atItem)                          , false ) ;
  nKickOut ( !IsMask(event->buttons(),Qt::LeftButton) , false ) ;
  dragPoint = event->pos()                                      ;
  nKickOut ( !atItem->isSelected()                    , false ) ;
  nKickOut ( !PassDragDrop                            , false ) ;
  return true                                                   ;
}

bool N::ColorGroups::fetchDrag(QMouseEvent * event)
{
  nKickOut ( !IsMask(event->buttons(),Qt::LeftButton) , false ) ;
  QPoint pos = event->pos()                                     ;
  pos -= dragPoint                                              ;
  return ( pos.manhattanLength() > qApp->startDragDistance() )  ;
}

QMimeData * N::ColorGroups::dragMime(void)
{
  return standardMime("colorgroup") ;
}

void N::ColorGroups::dragDone(Qt::DropAction dropIt,QMimeData * mime)
{
}

bool N::ColorGroups::finishDrag(QMouseEvent * event)
{
  return true ;
}

bool N::ColorGroups::acceptDrop(QWidget * source,const QMimeData * mime)
{
  return true ;
}

bool N::ColorGroups::dropNew(QWidget * source,const QMimeData * mime,QPoint pos)
{
  return true ;
}

bool N::ColorGroups::dropMoving(QWidget * source,const QMimeData * mime,QPoint pos)
{
  return true ;
}

bool N::ColorGroups::dropAppend(QWidget * source,const QMimeData * mime,QPoint pos)
{
  return true ;
}

QString N::ColorGroups::MimeType(const QMimeData * mime)
{
  return AbstractGui::MimeType(mime,"colorgroup/uuid;colorgroup/uuids") ;
}

UUIDs N::ColorGroups::MimeUuids(const QMimeData * mime,QString mimetype)
{
  UUIDs Uuids                            ;
  QByteArray data = mime->data(mimetype) ;
  if (data.size()<=0) return Uuids       ;
  if (mimetype=="colorgroup/uuid")       {
    SUID * suid = (SUID *)data.data()    ;
    Uuids << (*suid)                     ;
  } else
  if (mimetype=="colorgroup/uuids")      {
    SUID * suid  = (SUID *)data.data()   ;
    int    total = (int)suid[0]          ;
    for (int i=0;i<total;i++)            {
      Uuids << suid[i+1]                 ;
      plan->processEvents()              ;
    }                                    ;
  }                                      ;
  return Uuids                           ;
}

void N::ColorGroups::run(int T,ThreadData * d)
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

void N::ColorGroups::Load(ThreadData * d)
{
  nDropOut            ( ! IsContinue ( d )          ) ;
  WaitClear           (                             ) ;
  EnterSQL            ( SC , plan -> sql            ) ;
    QString Q                                         ;
    QString name                                      ;
    UUIDs   Uuids                                     ;
    SUID    uuid                                      ;
    int     colors                                    ;
    Uuids = SC . Uuids                                (
              PlanTable(ColorGroups)                  ,
              "uuid"                                  ,
              SC . OrderByAsc("id")                 ) ;
    foreach ( uuid , Uuids )                          {
      NewTreeWidgetItem ( it )                        ;
      colors = 0                                      ;
      name = SC . getName                             (
               PlanTable(Names)                       ,
               "uuid"                                 ,
               vLanguageId                            ,
               uuid                                 ) ;
      Q = SC . sql . SelectFrom                       (
            "colors"                                  ,
            PlanTable(ColorGroups)                    ,
            SC . WhereUuid ( uuid )                 ) ;
      if              ( SC . Fetch ( Q )            ) {
        colors = SC . Int ( 0 )                       ;
      }                                               ;
      it -> setText   ( 0 , name                    ) ;
      it -> setText   ( 1 , QString::number(colors) ) ;
      it -> setData   ( 0 , Qt::UserRole,uuid       ) ;
      setAlignments   ( it                          ) ;
      addTopLevelItem ( it                          ) ;
    }                                                 ;
  LeaveSQL            ( SC , plan -> sql            ) ;
  Alert               ( Done                        ) ;
}

bool N::ColorGroups::startup(void)
{
  start ( 10001 ) ;
  return true     ;
}

void N::ColorGroups::singleClicked(QTreeWidgetItem * item,int column)
{
  if (item==ItemEditing && column==ItemColumn) return ;
  removeOldItem ()                                    ;
}

void N::ColorGroups::doubleClicked(QTreeWidgetItem * item,int column)
{
  QLineEdit * le = NULL                     ;
  switch (column)                           {
    case 0                                  :
      le  = setLineEdit                     (
              item                          ,
              column                        ,
              SIGNAL(editingFinished())     ,
              SLOT  (nameFinished   ())   ) ;
      le -> show     (                    ) ;
      le -> setFocus ( Qt::TabFocusReason ) ;
    break                                   ;
    default                                 :
      removeOldItem ()                      ;
    break                                   ;
  }                                         ;
}

void N::ColorGroups::nameFinished(void)
{
  SUID        uuid = nTreeUuid ( ItemEditing,0          )  ;
  QLineEdit * name = Casting   ( QLineEdit  ,ItemWidget )  ;
  //////////////////////////////////////////////////////////
  if (IsNull(name))                                        {
    removeOldItem ( )                                      ;
    return                                                 ;
  }                                                        ;
  //////////////////////////////////////////////////////////
  QString cn = name->text()                                ;
  if (cn.length()>0)                                       {
    EnterSQL ( SC , plan->sql )                            ;
      QString Q                                            ;
      if (uuid<=0)                                         {
        uuid = SC.Unique(PlanTable(MajorUuid),"uuid",7799) ;
        SC.insertUuid                                      (
          PlanTable(MajorUuid)                             ,
          uuid                                             ,
          Types::ColorGroup                              ) ;
        SC.insertUuid(PlanTable(ColorGroups),uuid,"uuid")  ;
        ItemEditing->setData(0,Qt::UserRole,uuid)          ;
      }                                                    ;
      SC.assureName(PlanTable(Names),uuid,vLanguageId,cn)  ;
      ItemEditing->setText(0,cn               )            ;
    LeaveSQL ( SC , plan->sql )                            ;
  }                                                        ;
  removeOldItem (      )                                   ;
  Alert         ( Done )                                   ;
}

void N::ColorGroups::Insert(void)
{
  removeOldItem     (                       ) ;
  NewTreeWidgetItem ( IT                    ) ;
  addTopLevelItem   ( IT                    ) ;
  scrollToItem      ( IT                    ) ;
  IT->setData       ( 0  , Qt::UserRole , 0 ) ;
  doubleClicked     ( IT , 0                ) ;
}

bool N::ColorGroups::Menu(QPoint pos)
{
  QTreeWidgetItem * item = itemAt(pos)                      ;
  MenuManager       mm (this)                               ;
  QAction         * a                                       ;
  mm . add ( 101 , tr("Refresh")          )                 ;
  if (NotNull(item))                                        {
    mm . add ( 301 , tr("Values"        )   )               ;
    mm . add ( 302 , tr("Color analysis")   )               ;
  }                                                         ;
  DockingMenu ( mm )                                        ;
  mm . setFont(plan)                                        ;
  a = mm.exec()                                             ;
  if (IsNull(a)) return true                                ;
  switch (mm[a])                                            {
    case 101                                                :
      startup ( )                                           ;
    break                                                   ;
    case 301                                                :
      emit ValueList  ( item->text(0) , nTreeUuid(item,0) ) ;
    break                                                   ;
    case 302                                                :
      emit ShowColors ( item->text(0) , nTreeUuid(item,0) ) ;
    break                                                   ;
    default                                                 :
      if (RunDocking(mm,a)) return true                     ;
    break                                                   ;
  }                                                         ;
  return true                                               ;
}
