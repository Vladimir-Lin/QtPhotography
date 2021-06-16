#include <qtphotography.h>

#pragma message("GalleryView need to change into newer implementation")

N::GalleryView:: GalleryView ( QWidget * parent , Plan * p )
               : GroupView   (           parent ,        p )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::GalleryView::~GalleryView (void)
{
}

void N::GalleryView::Configure(void)
{
  type    = Types::Gallery                                    ;
  display = ThumbIcon                                         ;
  /////////////////////////////////////////////////////////////
  setWindowTitle               ( tr("Galleries management") ) ;
  setDragDropMode              ( DragDrop                   ) ;
  setViewMode                  ( IconMode                   ) ;
  setIconSize                  ( QSize(128,128)             ) ;
  setGridSize                  ( QSize(144,168)             ) ;
  setMovement                  ( Snap                       ) ;
  setResizeMode                ( Adjust                     ) ;
  setSelectionMode             ( ExtendedSelection          ) ;
  setWordWrap                  ( true                       ) ;
  setMouseTracking             ( true                       ) ;
  setWrapping                  ( true                       ) ;
  setTextElideMode             ( Qt::ElideNone              ) ;
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAsNeeded      ) ;
  setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded      ) ;
  setMinimumSize               ( QSize(172,180)             ) ;
  setDropFlag                  ( DropText     , false       ) ;
  setDropFlag                  ( DropUrls     , true        ) ;
  setDropFlag                  ( DropImage    , true        ) ;
  setDropFlag                  ( DropHtml     , false       ) ;
  setDropFlag                  ( DropColor    , false       ) ;
  setDropFlag                  ( DropTag      , false       ) ;
  setDropFlag                  ( DropPicture  , true        ) ;
  setDropFlag                  ( DropPeople   , false       ) ;
  setDropFlag                  ( DropVideo    , false       ) ;
  setDropFlag                  ( DropAlbum    , false       ) ;
  setDropFlag                  ( DropGender   , false       ) ;
  setDropFlag                  ( DropDivision , true        ) ;
  plan->setFont                ( this                       ) ;
}

QString N::GalleryView::MimeType(const QMimeData * mime)
{
  return AbstractGui::MimeType            (
           mime                           ,
           "picture/uuid;picture/uuids;"
           "division/uuid;division/uuids"
         )                                ;
}

UUIDs N::GalleryView::MimeUuids(const QMimeData * mime,QString mimetype)
{
  UUIDs      Uuids                                           ;
  QByteArray data = mime->data(mimetype)                     ;
  if (data.size()<=0) return Uuids                           ;
  if (mimetype=="picture/uuid"  ) Uuids << GetUuid  ( data ) ; else
  if (mimetype=="picture/uuids" ) Uuids  = GetUuids ( data ) ; else
  if (mimetype=="division/uuid" ) Uuids << GetUuid  ( data ) ; else
  if (mimetype=="division/uuids") Uuids  = GetUuids ( data ) ;
  return Uuids                                               ;
}

QString N::GalleryView::ToolTipTotoal(int total)
{
  return tr("%1 pictures in this gallery").arg(total) ;
}

int N::GalleryView::Counting(SqlConnection & connection,SUID u)
{
  GroupItems GI ( plan )           ;
  GI . AutoMap    = true           ;
  GI . GroupTable = GI.LookTable   (
           type                    ,
           Types::Picture          ,
           Groups::Subordination ) ;
  return GI . Count                (
           connection              ,
           u                       ,
           type                    ,
           Types::Picture          ,
           Groups::Subordination ) ;
}

bool N::GalleryView::JoinImage(SUID u,const QImage & image)
{
  return JoinGallery(u,image) ;
}

void N::GalleryView::GroupPositions(RMAPs & Position,UUIDs & Uuids)
{
  int Id = 1            ;
  SUID u                ;
  foreach (u,Uuids)     {
    Position [ u ] = Id ;
    Id ++               ;
  }                     ;
}

bool N::GalleryView::JoinPictures(SUID u,const UUIDs & Uuids)
{
  return JoinGallery(u,Uuids) ;
}

bool N::GalleryView::JoinUrls(SUID u,const QList<QUrl> & urls)
{
  dropAction = true                                       ;
  Bustle  ( )                                             ;
  UUIDs Uuids                                             ;
  PictureManager PM(plan)                                 ;
  for (int i=0;i<urls.count();i++)                        {
    SUID puid = 0                                         ;
    QString filename = urls[i].toLocalFile ( )            ;
    if (PM.Import(filename,puid)) Uuids << puid           ;
    plan -> showMessage(tr("Importing %1").arg(filename)) ;
    plan -> processEvents (                             ) ;
  }                                                       ;
  Vacancy ( )                                             ;
  dropAction = false                                      ;
  return JoinGallery ( u , Uuids )                        ;
}

void N::GalleryView::DeletePictures(QListWidgetItem * it)
{
  SUID u = nListUuid(it)                         ;
  QString M                                      ;
  M = tr("Delete [%1] pictures").arg(it->text()) ;
  plan->showMessage(M)                           ;
  setEnabled ( false          )                  ;
  GroupItems GI ( plan )                         ;
  EnterSQL   ( SC , plan->sql )                  ;
    GI . AutoMap    = true                       ;
    GI . GroupTable = GI . LookTable             (
                       Types  :: Gallery         ,
                       Types  :: Picture         ,
                       Groups :: Subordination ) ;
    GI . FullDetach                              (
           SC                                    ,
           u                                     ,
           Types  :: Gallery                     ,
           Types  :: Picture                     ,
           Groups :: Subordination             ) ;
  LeaveSQL   ( SC , plan->sql )                  ;
  setEnabled ( true           )                  ;
  Alert      ( Done           )                  ;
}

bool N::GalleryView::Menu(QPoint pos)
{
  nScopedMenu ( mm , this )                        ;
  QMenu           * me                             ;
  QMenu           * mc                             ;
  QAction         * aa                             ;
  QListWidgetItem * it                             ;
  SUID              u = 0                          ;
  //////////////////////////////////////////////////
  it = itemAt(pos)                                 ;
  if (NotNull(it) && !it->isSelected()) it = NULL  ;
  if (NotNull(it)) u = nListUuid(it)               ;
  //////////////////////////////////////////////////
  mm.add(101,tr("New gallery"))                    ;
  mm.add(201,tr("Reflush"          ))              ;
  if (NotNull(it)) mm.add(202,tr("Reflush item"))  ;
  mm.addSeparator()                                ;
  me = mm.addMenu(tr("Edit"      ))                ;
  if (NotNull(it))                                 {
    mc = mm.addMenu(tr("Categorize"))              ;
  }                                                ;
  mm.addSeparator()                                ;
  mm.add(301,tr("Copy to clipboard"))              ;
  mm.add(302,tr("Clear selection"  ))              ;
  mm.add(303,tr("Select all"       ))              ;
  //////////////////////////////////////////////////
  if (NotNull(it))                                 {
    mm.add(me,102,tr("Rename"))                    ;
    mm.add(me,103,tr("Delete this gallery"))       ;
    mm.addSeparator()                              ;
    mm.add(me,104,tr("Delete all pictures"))       ;
    mm.addSeparator()                              ;
  }                                                ;
  mm.add(me,211,tr("Multilingual translations"))   ;
  AdjustMenu(mm,me)                                ;
  //////////////////////////////////////////////////
  if (NotNull(it))                                 {
    mm.add(mc,501,tr("Constraints"))               ;
    mm.add(mc,502,tr("Rule tables"))               ;
  }                                                ;
  //////////////////////////////////////////////////
  mm . setFont ( plan                )             ;
  aa = mm.exec (                     )             ;
  nKickOut     ( IsNull(aa) , true   )             ;
  //////////////////////////////////////////////////
  UUIDs Tuids                                      ;
  switch (mm[aa])                                  {
    case 101                                       :
      New             (    )                       ;
    break                                          ;
    case 102                                       :
      Rename          (    )                       ;
    break                                          ;
    case 103                                       :
      Delete          (    )                       ;
    break                                          ;
    case 104                                       :
      DeletePictures  ( it )                       ;
    break                                          ;
    case 201                                       :
      startup         (    )                       ;
    break                                          ;
    case 202                                       :
      Refresh         ( it )                       ;
    break                                          ;
    case 211                                       :
      Tuids = ItemUuids()                          ;
      emit Translations(windowTitle(),Tuids)       ;
    break                                          ;
    case 301                                       :
      CopyToClipboard ( )                          ;
    break                                          ;
    case 302                                       :
      SelectNone      ( )                          ;
    break                                          ;
    case 303                                       :
      SelectAll       ( )                          ;
    break                                          ;
    case 501                                       :
      emit Constraints ( windowTitle() , u    )    ;
    break                                          ;
    case 502                                       :
      emit RuleTables  ( windowTitle() , u    )    ;
    break                                          ;
    default                                        :
      RunAdjustment    ( mm            , aa   )    ;
    break                                          ;
  }                                                ;
  return true                                      ;
}
