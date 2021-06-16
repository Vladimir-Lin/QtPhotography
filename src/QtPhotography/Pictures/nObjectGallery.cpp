#include <qtphotography.h>

#define hasMenu   isFunction(1001)
#define ALERT(ID) AnotherALERT(N::Command::ID)

N::ObjectGallery:: ObjectGallery  ( QWidget * parent , Plan * p )
                 : PictureView    (           parent ,        p )
{
  WidgetClass       ;
  t1 = Types::Album ;
}

N::ObjectGallery::~ObjectGallery(void)
{
}

void N::ObjectGallery::joinPictures(SUID before,UUIDs Uuids)
{
  SqlConnection SC ( plan -> sql )    ;
  if ( SC . open ( FunctionString ) ) {
    GroupItems :: Join                (
      SC                              ,
      first                           ,
      t1                              ,
      t2                              ,
      relation                        ,
      before                          ,
      Uuids                         ) ;
    SC . close  (      )              ;
  }                                   ;
  SC   . remove (      )              ;
  Reload        ( true )              ;
}

void N::ObjectGallery::joinPictures(SUID before,QList<QUrl> urls)
{
  setEnabled        (false    )                 ;
  SqlConnection  SC (plan->sql)                 ;
  if ( SC . open ( FunctionString ) )           {
    UUIDs Uuids                                 ;
    for (int i=0;i<urls.count();i++)            {
      QString filename = urls[i].toLocalFile()  ;
      QImage  Image                             ;
      if (Image.load(filename))                 {
        QByteArray Body                         ;
        Body.clear()                            ;
        QFile F(filename)                       ;
        if (F.open(QIODevice::ReadOnly))        {
          Body = F.readAll()                    ;
          F.close()                             ;
        }                                       ;
        if (Body.size()>0)                      {
          SUID u                                ;
          u = PictureManager :: Find(SC,Body)   ;
          if (u<=0)                             {
            PictureManager::Import(filename,u)  ;
          }                                     ;
          if (u>0) Uuids << u                   ;
        }                                       ;
      }                                         ;
    }                                           ;
    if (Uuids.count()>0)                        {
      GroupItems :: Join                        (
        SC                                      ,
        first                                   ,
        t1                                      ,
        t2                                      ,
        relation                                ,
        before                                  ,
        Uuids                                 ) ;
    }                                           ;
    SC.close  (      )                          ;
  }                                             ;
  SC . remove (      )                          ;
  setEnabled  ( true )                          ;
  Reload      ( true )                          ;
}

void N::ObjectGallery::JoinImage(const QMimeData * mime)
{
  QImage * I = PictureManager :: MimeImage ( mime ) ;
  if (IsNull(I)) return                             ;
  SUID puid    = 0                                  ;
  bool success = false                              ;
  success = PictureManager :: Import ( *I , puid )  ;
  delete I                                          ;
  I = NULL                                          ;
  if (puid<=0 ) return                              ;
  UUIDs Uuids                                       ;
  Uuids << puid                                     ;
  joinPictures ( 0 , Uuids )                        ;
}

bool N::ObjectGallery::dropImage(QWidget * source,QPointF psf,const QImage & image)
{
  if (source==this) return true             ;
  QPoint pos = psf.toPoint()                ;
  QListWidgetItem * atItem = itemAt(pos)    ;
  SUID atUuid = 0                           ;
  if (NotNull(atItem))                      {
    atUuid = nListUuid(atItem)              ;
  }                                         ;
  SUID puid    = 0                          ;
  bool success = false                      ;
  QImage I = image                          ;
  success = PictureManager::Import(I,puid)  ;
  if ( puid <= 0     ) return true          ;
  if ( ! isOwner ( ) ) return true          ;
  dropAction = true                         ;
  UUIDs Uuids                               ;
  Uuids << puid                             ;
  SqlConnection SC(plan->sql)               ;
  if ( SC . open ( FunctionString ) )       {
    GroupItems::Join                        (
      SC                                    ,
      first                                 ,
      t1                                    ,
      t2                                    ,
      relation                              ,
      atUuid                                ,
      Uuids                               ) ;
    SC.close()                              ;
  }                                         ;
  SC.remove()                               ;
  ALERT ( Done )                            ;
  dropAction = false                        ;
  return true                               ;
}

bool N::ObjectGallery::dropPictures(QWidget * source,QPointF psf,const UUIDs & Uuids)
{
  QPoint pos = psf.toPoint()                      ;
  QListWidgetItem * atItem = itemAt(pos)          ;
  if ( source == this )                           {
    if (IsNull(atItem)) return true               ;
    SUID auid = nListUuid(atItem)                 ;
    MovePicture ( auid , Uuids )                  ;
  } else                                          {
    if ( ! isOwner ( ) ) return true              ;
    SUID  auid = 0                                ;
    UUIDs ITEMs = ItemUuids()                     ;
    UUIDs AUIDs                                   ;
    SUID  uuid                                    ;
    if (NotNull(atItem)) auid = nListUuid(atItem) ;
    foreach (uuid,Uuids)                          {
      if (!ITEMs.contains(uuid)) AUIDs << uuid    ;
    }                                             ;
    if (AUIDs.count()<=0) return true             ;
    joinPictures ( auid , AUIDs )                 ;
  }                                               ;
  return true                                     ;
}

void N::ObjectGallery::MovePicture(SUID atUuid,const UUIDs & Uuids)
{
  dropAction = true                            ;
  SUID              uuid                       ;
  UUIDs             ITEMs = ItemUuids ( )      ;
  ListWidgetItems mItems                       ;
  foreach (uuid,Uuids)                         {
    QListWidgetItem * it = uuidAt ( uuid )     ;
    mItems << it                               ;
    takeItem ( row ( it ) )                    ;
    ITEMs . takeAt ( ITEMs . indexOf (uuid) )  ;
  }                                            ;
  QListWidgetItem * rit   = uuidAt ( atUuid )  ;
  int               rid   = row    ( rit    )  ;
  for (int i=0;i<mItems.count();i++,rid++)     {
    insertItem ( rid , mItems [ i ] )          ;
  }                                            ;
  //////////////////////////////////////////////
  UUIDs Puids = Uuids                          ;
  Bustle ( )                                   ;
  SqlConnection SC(plan->sql)                  ;
  if ( SC . open ( FunctionString ) )          {
    if ( isOwner ( ) )                         {
      GroupItems :: Join                       (
        SC                                     ,
        first                                  ,
        t1                                     ,
        t2                                     ,
        relation                               ,
        atUuid                                 ,
        Puids                                ) ;
    }                                          ;
    SC.close()                                 ;
  }                                            ;
  SC.remove()                                  ;
  //////////////////////////////////////////////
  Vacancy  ( )                                 ;
  ALERT ( Done )                               ;
  dropAction = false                           ;
}

bool N::ObjectGallery::Import(QStringList Files)
{
  PictureView::Import ( Files ) ;
  Reload              ( true  ) ;
  return true                   ;
}

void N::ObjectGallery::ImportFaces(void)
{
  nDropOut ( ! isOwner ( )   )            ;
  EnterSQL ( SC , plan->sql  )            ;
    QString Q                             ;
    UUIDs   Uuids                         ;
    UUIDs   Fuids                         ;
    SUID    uuid                          ;
    Uuids = Subordination                 (
              SC                          ,
              first                       ,
              t1                          ,
              t2                          ,
              relation                    ,
              SC.OrderByAsc("position") ) ;
    foreach (uuid,Uuids)                  {
      Q = SC.sql.SelectFrom               (
            "uuid"                        ,
            PlanTable(Faces)              ,
            QString( "where picture = %1" )
            .arg(uuid)                  ) ;
      SqlLoopNow ( SC , Q )               ;
        SUID fuid = SC.Uuid(0)            ;
        if (!Fuids.contains(fuid))        {
          Fuids << fuid                   ;
        }                                 ;
      SqlLoopErr ( SC , Q )               ;
      SqlLoopEnd ( SC , Q )               ;
    }                                     ;
    if (Fuids.count()>0)                  {
      GroupItems::Join                    (
        SC                                ,
        first                             ,
        t1                                ,
        Types::Face                       ,
        relation                          ,
        0                                 ,
        Fuids                           ) ;
    }                                     ;
  LeaveSQL ( SC , plan->sql  )            ;
  ALERT    ( Done            )            ;
}

void N::ObjectGallery::RecognizeFaces(void)
{
}

bool N::ObjectGallery::RunCommand(VarArgs & arguments)
{
  return PictureView :: RunCommand ( arguments ) ;
}

bool N::ObjectGallery::Menu(QPoint pos)
{
  nKickOut ( ! hasMenu     , false )                            ;
  nKickOut ( ! isOwner ( ) , false )                            ;
  ///////////////////////////////////////////////////////////////
  MenuManager       mm(this)                                    ;
  QMenu           * ma                                          ;
  QMenu           * me                                          ;
  QMenu           * mi                                          ;
  QMenu           * ms                                          ;
  QAction         * aa                                          ;
  QListWidgetItem * it                                          ;
  SUID              uuid     = 0                                ;
  UUIDs             Uuids                                       ;
  int               StartId  = 0                                ;
  int               PageSize = 0                                ;
  ///////////////////////////////////////////////////////////////
  it = itemAt(pos)                                              ;
  if (NotNull(it) && !it->isSelected()) it = NULL               ;
  if (NotNull(it)) uuid = nListUuid(it)                         ;
  Uuids = ListWidget :: Selected ( )                            ;
  ///////////////////////////////////////////////////////////////
  addPictureMenu ( mm , it , uuid , StartId , PageSize )        ;
  mm . addSeparator ( )                                         ;
  ///////////////////////////////////////////////////////////////
  me = mm . addMenu ( tr("Edit"       )    )                    ;
  mi = mm . addMenu ( tr("Information")    )                    ;
  addCatalogMenu    ( mm ,      it , Uuids )                    ;
  addEditingMenu    ( mm , me , it , Uuids )                    ;
  addProcessMenu    ( mm ,      it , Uuids )                    ;
  ///////////////////////////////////////////////////////////////
  if ( Types::People == t1 )                                    {
    mm.add(me,705,tr("Import related human faces"))             ;
    mm.add(me,706,tr("Facial recognition"))                     ;
    mm.addSeparator(me)                                         ;
  }                                                             ;
  if ( Types::Album == t1 )                                     {
  }                                                             ;
  mm.add(me,203,tr("Set font"         ))                        ;
  mm.addSeparator()                                             ;
  ma = mm . addMenu ( tr("Adjustments") )                       ;
  ms = mm . addMenu ( tr("Selections" ) )                       ;
  mm . add ( ms,311,tr("Select pictures by related webpages") ) ;
  mm . add ( ms,312,tr("Select pictures by sets algebra"    ) ) ;
  mm . addSeparator ( ms                   )                    ;
  SelectionsMenu    ( mm , ms              )                    ;
  addDetailsMenu    ( mm , mi , it , Uuids )                    ;
  AdjustMenu        ( mm , ma              )                    ;
  DockingMenu       ( mm                   )                    ;
  ///////////////////////////////////////////////////////////////
  mm . setFont   ( plan )                                       ;
  aa = mm . exec (      )                                       ;
  if (IsNull(aa))                                               {
    PageChanged(StartId,PageSize)                               ;
    return false                                                ;
  }                                                             ;
  ///////////////////////////////////////////////////////////////
  if ( NotNull(it) ) setCurrentItem ( it )                      ;
  if ( RunDocking     ( mm , aa        ) ) return true          ;
  if ( RunAdjustment  ( mm , aa        ) ) return true          ;
  if ( RunSelections  ( mm , aa        ) ) return true          ;
  if ( RunPictureMenu ( mm , aa , it   ) ) return true          ;
  if ( RunEditingMenu ( mm,aa,it,Uuids ) ) return true          ;
  if ( RunCatalogMenu ( mm,aa,it,Uuids ) ) return true          ;
  if ( RunDetailsMenu ( mm,aa,it,Uuids ) ) return true          ;
  if ( RunProcessMenu ( mm,aa,it,Uuids ) ) return true          ;
  ///////////////////////////////////////////////////////////////
  switch (mm[aa])                                               {
    case 203                                                    :
      setFont            (                                    ) ;
    return true                                                 ;
    case 705                                                    :
      ImportFaces        (                                    ) ;
    return true                                                 ;
    case 706                                                    :
      RecognizeFaces     (                                    ) ;
    return true                                                 ;
  }                                                             ;
  RunPageMenu ( mm , aa , StartId , PageSize )                  ;
  PageChanged ( StartId , PageSize           )                  ;
  return true                                                   ;
}
