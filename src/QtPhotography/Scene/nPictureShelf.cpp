#include <qtphotography.h>

#ifdef QT_OPENGL_LIB

N::PictureShelf:: PictureShelf ( QObject * parent , Plan * p )
                : ViewShelf    (           parent ,        p )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::PictureShelf:: PictureShelf ( QObject * parent , Plan * p , SceneDestructor * des )
                : ViewShelf    (           parent ,        p ,                   des )
{
  Configure ( ) ;
}

N::PictureShelf::~PictureShelf (void)
{
}

void N::PictureShelf::Configure(void)
{
  QObject::setProperty ( "Type"  , "PictureShelf"                      ) ;
  setVariable          ( "Type"  , "PictureShelf"                      ) ;
  setVariable          ( "Menu"  , true                                ) ;
  ////////////////////////////////////////////////////////////////////////
  setVariable          ( "Access"           , "First"                  ) ;
  setVariable          ( "Table"            , PlanTable(PictureOrders) ) ;
  setVariable          ( "T1"               , Types::Gallery           ) ;
  setVariable          ( "T2"               , Types::Picture           ) ;
  setVariable          ( "T3"               , Types::Gallery           ) ;
  setVariable          ( "T4"               , Types::Gallery           ) ;
  setVariable          ( "T5"               , Types::Picture           ) ;
  setVariable          ( "Sorting"          , Qt::AscendingOrder       ) ;
  setVariable          ( "Items"            , 0                        ) ;
  setVariable          ( "AutoRefresh"      , true                     ) ;
  setVariable          ( "ReplaceIcon"      , ":/images/pictures.png"  ) ;
  setVariable          ( "ArrangeBy"        , "Picture"                ) ;
  setVariable          ( "ScrollBy"         , "Start"                  ) ;
  ////////////////////////////////////////////////////////////////////////
  setVariable          ( "Editable"         , true                     ) ;
//  setVariable          ( "FollowWindow"     , true                     ) ;
  ////////////////////////////////////////////////////////////////////////
  setDropFlag          ( DropText           , false                    ) ;
  setDropFlag          ( DropUrls           , false                    ) ;
  setDropFlag          ( DropImage          , false                    ) ;
  setDropFlag          ( DropHtml           , false                    ) ;
  setDropFlag          ( DropColor          , false                    ) ;
  setDropFlag          ( DropTag            , false                    ) ;
  setDropFlag          ( DropPicture        , true                     ) ;
  setDropFlag          ( DropPeople         , false                    ) ;
  setDropFlag          ( DropVideo          , false                    ) ;
  setDropFlag          ( DropAlbum          , false                    ) ;
  setDropFlag          ( DropGender         , false                    ) ;
  setDropFlag          ( DropDivision       , false                    ) ;
}

bool N::PictureShelf::FocusIn(void)
{
  if ( IsNull ( plan ) ) return false    ;
  return true                            ;
}

bool N::PictureShelf::ReportDetails(void)
{
  return false ;
}

bool N::PictureShelf::dropPictures (
       QWidget     * widget        ,
       ControlPoint  pos           ,
       const UUIDs & Uuids         )
{
  UUIDs   U = Uuids                                  ;
  SUID    p = Value ( "DropUUID" ) . toULongLong ( ) ;
  VarArgs args                                       ;
  if ( U . count ( ) <= 0 ) return true              ;
  args << p                                          ;
  toVariants ( U , args )                            ;
//  start ( CiosMenuId ( 85 , 12 , 25 ) , args )       ;
  return true                                        ;
}

// PickView::ItemRunner     => 10001 , 10002 , 10003 , 10004 , 10011, 73002, 98011, 98012, 851226, 851227, 851228, 851229, 851230, 851231, 851232
// GalleryShelf::ItemRunner => 73007 , 73008
bool N::PictureShelf::ItemRunner(int type,ThreadData * data)
{
  if ( PickView::ItemRunner ( type , data ) ) return true                    ;
  switch ( type )                                                            {
    case CiosMenuId   ( 89 , 44 , 14      )                                  : // Assign as group icon
    return true                                                              ;
    case CiosMenuId   ( 89 , 44 , 15      )                                  : // Remove group icon
    return true                                                              ;
    case CiosMenuId   ( 89 , 44 , 18      )                                  :
      AllPictureSizes ( data -> Arguments )                                  ;
    return true                                                              ;
    case CiosMenuId   ( 89 , 44 , 51      )                                  :
      PictureSize     ( data -> Arguments )                                  ;
    return true                                                              ;
    case CiosMenuId   ( 89 , 44 , 52      )                                  :
      PictureOrderId  ( data -> Arguments )                                  ;
    return true                                                              ;
  }                                                                          ;
  return false                                                               ;
}

void N::PictureShelf::atDoubleClicked(SelectItem * item)
{
  PushCommand ( CiosMenuId ( 89 , 44 , 66 ) , item ) ;
}

void N::PictureShelf::PictureSize(VarArgs & args)
{
}

void N::PictureShelf::PictureOrderId(VarArgs & args)
{
}

void N::PictureShelf::AllPictureSizes(VarArgs & args)
{
}

void N::PictureShelf::AllPictures(void)
{
  emit ShowAll ( ) ;
}

void N::PictureShelf::ImportPictures(void)
{
}

void N::PictureShelf::SelectNothing(void)
{
  UUIDs U = CurrentItems                          ;
  SUID  u                                         ;
  foreach ( u , U )                               {
    SelectItem * item = ChildOf<SelectItem> ( u ) ;
    if ( NotNull ( item ) )                       {
      item -> setChecked ( false )                ;
    }                                             ;
  }                                               ;
  setVariable ( "CurrentNode" , 0 )               ;
}

void N::PictureShelf::SelectAll(void)
{
  UUIDs U = CurrentItems                          ;
  SUID  u                                         ;
  foreach ( u , U )                               {
    SelectItem * item = ChildOf<SelectItem> ( u ) ;
    if ( NotNull ( item ) )                       {
      item -> setChecked ( true )                 ;
    }                                             ;
  }                                               ;
}

int N::PictureShelf::Indexing(RangeBar * rbu,int direction,QVariants & v)
{
//  qint64 s = v [ 0 ] . toLongLong ( ) ; // start
//  qint64 e = v [ 1 ] . toLongLong ( ) ; // end
//  qint64 p = v [ 2 ] . toLongLong ( ) ; // page
  qint64 i = v [ 3 ] . toLongLong ( ) ; // index
  setVariable ( "StartId" , i )       ;
  start       ( 10011         )       ;
  return 0                            ;
}

bool N::PictureShelf::Relocation(QRectF viewport)
{
  return CentralRelocation ( viewport ) ;
}

void N::PictureShelf::PictureMenu(MenuManager & mm,QMenu * me,SelectItem * item)
{
  UUIDs U = SelectedItems ( )                                                ;
  ////////////////////////////////////////////////////////////////////////////
  mm          . add          ( me                                            ,
                               CiosMenuId ( 89 , 44 , 17 )                   ,
                               tr("Show all pictures")                     ) ;
  mm          . add          ( me                                            ,
                               CiosMenuId ( 89 , 44 , 18 )                   ,
                               tr("Show size of all pictures")             ) ;
  if ( FirstUuid ( ) > 0 )                                                   {
    if ( NotNull ( item ) )                                                  {
      mm      . add          ( me                                            ,
                               CiosMenuId(89,44,41)                          ,
                               tr("Remove")                                ) ;
    }                                                                        ;
    if ( U . count ( ) > 0 )                                                 {
      mm      . add          ( me                                            ,
                               CiosMenuId(89,44,41)                          ,
                               tr("Remove selections")                     ) ;
    }                                                                        ;
    mm        . addSeparator ( me                                          ) ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  if ( NotNull ( item ) )                                                    {
    mm        . add          ( me                                            ,
                               CiosMenuId(89,44,41)                          ,
                               tr("Documents")                             ) ;
    mm        . add          ( me                                            ,
                               CiosMenuId(89,44,41)                          ,
                               tr("Download from related webpage")         ) ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  if ( U . count ( ) > 0 )                                                   {
    mm   . addSeparator ( me )                                               ;
    if ( FirstUuid() > 0 )                                                   {
      mm      . add          ( me                                            ,
                               70401                                         ,
                               tr("Move selected pictures to group end")   ) ;
    }                                                                        ;
    mm        . add          ( me                                            ,
                               70402                                         ,
                               tr("Move selected pictures to depot end")   ) ;
    mm        . addSeparator ( me                                          ) ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  if ( FirstUuid() > 0 )                                                     {
    mm        . add          ( me                                            ,
                               70403                                         ,
                               tr("Move all pictures in this gallery to depot end") ) ;
    mm        . addSeparator ( me                                          ) ;
    mm        . add          ( me                                            ,
                               70410                                         ,
                               tr("Relocate picture orders of this group") ) ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  mm          . add         ( me                                             ,
                              CiosMenuId(89,44,41)                           ,
                              tr("Relocate picture depot ID orders")       ) ;
}

void N::PictureShelf::DetailsMenu(MenuManager & mm,QMenu * md,SelectItem * item)
{
  if ( IsNull ( item ) ) return                                            ;
  if ( IsNull ( md   ) ) return                                            ;
  QLineEdit * utem     = NULL                                              ;
  SUID        itemUuid = item -> Value ( "UUID"     ) . toULongLong ( )    ;
  QString     uuidName = QString::number(itemUuid)                         ;
  utem             = new QLineEdit (                                     ) ;
  utem            -> setText       ( uuidName                            ) ;
  mm      . add          ( md , CiosMenuId(89,44,62) , utem              ) ;
  mm      . addSeparator ( md                                            ) ;
  //////////////////////////////////////////////////////////////////////////
  mm      . add          ( md                                              ,
                           CiosMenuId(89,44,51)                            ,
                           tr("Show picture size")                       ) ;
  mm      . add          ( md                                              ,
                           CiosMenuId(89,44,52)                            ,
                           tr("Picture order Id")                        ) ;
  mm      . add          ( md                                              ,
                           CiosMenuId(89,44,71)                            ,
                           tr("Related web pages")                       ) ;
  mm      . add          ( md                                              ,
                           CiosMenuId(89,44,72)                            ,
                           tr("Related people")                          ) ;
  mm      . add          ( md                                              ,
                           CiosMenuId(89,44,72)                            ,
                           tr("All related items")                       ) ;
  mm      . add          ( md                                              ,
                           CiosMenuId(89,44,72)                            ,
                           tr("Report picture URLs")                     ) ;
}

void N::PictureShelf::CatalogMenu(MenuManager & mm,QMenu * mc)
{
  if ( IsNull ( mc ) ) return                                       ;
  mm . add ( mc , CiosMenuId ( 89 , 44 , 81 ) , tr("Constraints") ) ;
  mm . add ( mc , CiosMenuId ( 89 , 44 , 82 ) , tr("Rule tables") ) ;
  mm . add ( mc , CiosMenuId ( 89 , 44 , 83 ) , tr("Subgroups"  ) ) ;
}

void N::PictureShelf::FiltersMenu(MenuManager & mm,QMenu * mc)
{
  if ( IsNull ( mc ) ) return                                       ;
  mm . add ( mc , CiosMenuId ( 89 , 64 , 31 ) , tr("Filters") ) ;
  mm . add ( mc , CiosMenuId ( 89 , 64 , 32 ) , tr("Facial recognition") ) ;
  mm . add ( mc , CiosMenuId ( 89 , 64 , 33 ) , tr("Stitching selected pictures"  ) ) ;
}

bool N::PictureShelf::PictureProcessor(MenuManager & mm,QAction * aa,SelectItem * item)
{
  switch ( mm [ aa ] )                              {
    case CiosMenuId ( 89 , 44 , 17                ) : // Show all pictures
      AllPictures   (                             ) ;
    return true                                     ;
    case CiosMenuId ( 89 , 44 , 18                ) : // Show picture size
      start         ( CiosMenuId ( 89 , 44 , 18 ) ) ;
    return true                                     ;
  }                                                 ;
  return false                                      ;
}

bool N::PictureShelf::DetailsProcessor(MenuManager & mm,QAction * aa,SelectItem * item)
{
  if ( IsNull ( item ) ) return false                        ;
  SUID itemUuid = item -> Value ( "UUID" ) . toULongLong ( ) ;
  VarArgs args                                               ;
  switch ( mm [ aa ] )                                       {
    case CiosMenuId ( 89 , 44 , 51 )                         : // Show picture size
      args << itemUuid                                       ;
      start ( CiosMenuId ( 89 , 44 , 51 ) , args )           ;
    return true                                              ;
    case CiosMenuId ( 89 , 44 , 52 )                         : // Picture order Id
      args << itemUuid                                       ;
      start ( CiosMenuId ( 89 , 44 , 52 ) , args )           ;
    return true                                              ;
  }                                                          ;
  return false                                               ;
}

bool N::PictureShelf::CatalogProcessor(MenuManager & mm,QAction * aa)
{
  switch ( mm [ aa ] )               {
    case CiosMenuId ( 89 , 44 , 81 ) : // Constraints
    return true                      ;
    case CiosMenuId ( 89 , 44 , 82 ) : // Rule tables
    return true                      ;
    case CiosMenuId ( 89 , 44 , 83 ) : // Subgroups
    return true                      ;
  }                                  ;
  return false                       ;
}

bool N::PictureShelf::FiltersProcessor(MenuManager & mm,QAction * aa)
{
  switch ( mm [ aa ] )               {
    case CiosMenuId ( 89 , 64 , 31 ) : // Filters
    return true                      ;
    case CiosMenuId ( 89 , 64 , 32 ) : // Facial recognition
    return true                      ;
    case CiosMenuId ( 89 , 64 , 33 ) : // Stitching selected pictures
    return true                      ;
  }                                  ;
  return false                       ;
}

bool N::PictureShelf::Menu   (
       QWidget      * widget ,
       QPointF        pos    ,
       ControlPoint & From   ,
       ControlPoint & To     )
{
  AllowPickItemMenu                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  MenuManager  mm ( widget )                                                 ;
  QAction    * aa   = NULL                                                   ;
  QMenu      * ma   = NULL                                                   ;
  QMenu      * ms   = NULL                                                   ;
  QMenu      * md   = NULL                                                   ;
  QMenu      * me   = NULL                                                   ;
  QMenu      * mc   = NULL                                                   ;
  QMenu      * mp   = NULL                                                   ;
  SelectItem * item = NearestItem ( From , To )                              ;
  VarArgs      args                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  if ( IsNull ( item ) ) item = CurrentItem ( )                              ;
  ////////////////////////////////////////////////////////////////////////////
  if ( hasFunction ( "Editable" ) )                                          {
    if ( NotNull ( item ) )                                                  {
      mm    . add          ( CiosMenuId(89,44,13) , tr("Open picture")     ) ;
    }                                                                        ;
    mm      . add          ( CiosMenuId(89,44,12) , tr("Refresh"     ) )     ;
    mm      . addSeparator (                                           )     ;
    if ( FirstUuid ( ) > 0 )                                                 {
      if ( NotNull ( item ) )                                                {
        mm  . add      ( CiosMenuId(89,44,14) , tr("Assign as group icon") ) ;
      }                                                                      ;
      mm    . add      ( CiosMenuId(89,44,15) , tr("Remove group icon")    ) ;
    }                                                                        ;
    mm      . add          ( CiosMenuId(89,44,16) , tr("Import pictures")  ) ;
    mm      . addSeparator (                                           )     ;
    me = mm . addMenu      ( tr("Edit")                                )     ;
    PictureMenu            ( mm , me , item                                ) ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  if ( hasFunction ( "Selectable" ) && NotNull ( item ) )                    {
    md = mm . addMenu      ( tr("Details")                                 ) ;
    mp = mm . addMenu      ( tr("Image processing")                        ) ;
    DetailsMenu            ( mm , md , item                                ) ;
    FiltersMenu            ( mm , mp                                       ) ;
    if ( FirstUuid ( ) > 0 )                                                 {
      mc = mm . addMenu    ( tr("Categorize")                              ) ;
      CatalogMenu          ( mm , mc                                       ) ;
    }                                                                        ;
    mm      . addSeparator (                                               ) ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  ma   = mm . addMenu  ( tr("Adjustments")    )                              ;
  ms   = mm . addMenu  ( tr("Sorting")        )                              ;
  AdjustMenu           ( mm , ma              )                              ;
  SortingMenu          ( mm , ms              )                              ;
  ////////////////////////////////////////////////////////////////////////////
  mm . addSeparator    (                      )                              ;
  mm . add             ( CiosMenuId(89,44,99)                                ,
                         tr("Close pictures explorer")                     ) ;
  ////////////////////////////////////////////////////////////////////////////
  RunPickItemMenu                                                            ;
  nKickOut             ( IsNull ( aa ) , true )                              ;
  ////////////////////////////////////////////////////////////////////////////
  if ( AdjustProcessor  ( mm , aa        ) ) return true                     ;
  if ( SortingProcessor ( mm , aa        ) ) return true                     ;
  if ( CatalogProcessor ( mm , aa        ) ) return true                     ;
  if ( FiltersProcessor ( mm , aa        ) ) return true                     ;
  if ( DetailsProcessor ( mm , aa , item ) ) return true                     ;
  if ( PictureProcessor ( mm , aa , item ) ) return true                     ;
  if ( 0 != CallToMenu  ( mm , aa , true ) ) return true                     ;
  ////////////////////////////////////////////////////////////////////////////
  switch ( mm [ aa ] )                                                       {
    case CiosMenuId    ( 89 , 44 , 99 )                                      : // Close this picture explorer
      emit CloseObject ( node         )                                      ;
    break                                                                    ;
    case CiosMenuId    ( 89 , 44 , 12 )                                      : // Refresh
      startup          (              )                                      ;
    break                                                                    ;
    case CiosMenuId    ( 89 , 44 , 13 )                                      : // Open picture
      if ( NotNull ( item ) )                                                {
        emit DoubleClicked ( item )                                          ;
        atDoubleClicked    ( item )                                          ;
      }                                                                      ;
    break                                                                    ;
    case CiosMenuId    ( 89 , 44 , 14 )                                      : // Assign as group icon
      if ( NotNull ( item ) )                                                {
        args << item -> Value ( "UUID" ) . toULongLong ( )                   ;
        start          ( CiosMenuId ( 89 , 44 , 14 ) , args )                ;
      }                                                                      ;
    break                                                                    ;
    case CiosMenuId    ( 89 , 44 , 15 )                                      : // Remove group icon
      start            ( CiosMenuId ( 89 , 44 , 15 ) )                       ;
    break                                                                    ;
    case CiosMenuId    ( 89 , 44 , 16 )                                      : // Import pictures
      ImportPictures   (              )                                      ;
    break                                                                    ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  return true                                                                ;
}

#endif
