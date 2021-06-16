#include <qtphotography.h>

N::ColorLists:: ColorLists      ( QWidget * parent , Plan * p )
              : TreeDock        (           parent ,        p )
              , Object          (                             )
              , GraphicsManager (                           p )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::ColorLists::~ColorLists(void)
{
}

void N::ColorLists::Configure(void)
{
  model = Graphics::RGBA                                        ;
  ///////////////////////////////////////////////////////////////
  setAccessibleName        ( "N::ColorLists"                  ) ;
  setObjectName            ( "N::ColorLists"                  ) ;
  ///////////////////////////////////////////////////////////////
  setWindowTitle           ( tr("Color value list")           ) ;
  setWindowIcon            ( QIcon(":/images/colorlists.png") ) ;
  ///////////////////////////////////////////////////////////////
  setRootIsDecorated       ( false                            ) ;
  setAlternatingRowColors  ( true                             ) ;
  setSelectionMode         ( ExtendedSelection                ) ;
  setColorHeader           (                                  ) ;
  ///////////////////////////////////////////////////////////////
  plan -> setFont          ( this                             ) ;
  setDragDropMode          ( DragDrop                         ) ;
  setDropFlag              ( DropColor   , true               ) ;
  setDropFlag              ( DropImage   , true               ) ;
  setDropFlag              ( DropPicture , true               ) ;
  ///////////////////////////////////////////////////////////////
  nConnect ( this , SIGNAL ( Refresh ( ) )                      ,
             this , SLOT   ( List    ( ) )                    ) ;
}

QSize N::ColorLists::sizeHint(void) const
{
  return SizeSuggestion ( QSize ( 320 , 480 ) ) ;
}

bool N::ColorLists::FocusIn(void)
{
  nKickOut          ( IsNull ( plan ) , false           ) ;
  DisableAllActions (                                   ) ;
  AssignAction      ( Label           , windowTitle ( ) ) ;
  LinkAction        ( Delete          , Delete      ( ) ) ;
  LinkAction        ( Export          , Export      ( ) ) ;
  LinkAction        ( Save            , Save        ( ) ) ;
  LinkAction        ( Insert          , Insert      ( ) ) ;
  return true                                             ;
}

bool N::ColorLists::hasItem(void)
{
  QTreeWidgetItem * item = currentItem () ;
  return NotNull ( item )                 ;
}

bool N::ColorLists::startDrag(QMouseEvent * event)
{
  QTreeWidgetItem * atItem = itemAt(event->pos())               ;
  nKickOut (  IsNull(atItem)                          , false ) ;
  nKickOut ( !IsMask(event->buttons(),Qt::LeftButton) , false ) ;
  dragPoint = event->pos()                                      ;
  nKickOut ( !atItem->isSelected()                    , false ) ;
  nKickOut ( !PassDragDrop                            , false ) ;
  return true                                                   ;
}

bool N::ColorLists::fetchDrag(QMouseEvent * event)
{
  nKickOut ( !IsMask(event->buttons(),Qt::LeftButton) , false ) ;
  QPoint pos = event->pos()                                     ;
  pos -= dragPoint                                              ;
  return ( pos.manhattanLength() > qApp->startDragDistance() )  ;
}

QMimeData * N::ColorLists::dragMime(void)
{
  QTreeWidgetItem * item = currentItem ()              ;
  if (IsNull(item)) return NULL                        ;
  QRgb rgb = (QRgb)item->data(0,Qt::UserRole).toUInt() ;
  QMimeData * mime = new QMimeData()                   ;
  mime->setColorData(QVariant(QColor(rgb)))            ;
  return mime                                          ;
}

void N::ColorLists::dragDone(Qt::DropAction dropIt,QMimeData * mime)
{
}

bool N::ColorLists::finishDrag(QMouseEvent * event)
{
  return true ;
}

bool N::ColorLists::acceptDrop(QWidget * source,const QMimeData * mime)
{
  if (source==this) return false ;
  return dropHandler ( mime )    ;
}

bool N::ColorLists::dropNew(QWidget * source,const QMimeData * mime,QPoint pos)
{
  if (source==this) return false       ;
  QString    mtype                     ;
  UUIDs      Uuids                     ;
  mtype = MimeType  (mime      )       ;
  Uuids = MimeUuids (mime,mtype)       ;
  bool correct = false                 ;
  if (Uuids.count()>0 ) correct = true ;
  if (mime->hasColor()) correct = true ;
  if (mime->hasImage()) correct = true ;
  if (!correct) return false           ;
  Alert ( Action )                     ;
  return true                          ;
}

bool N::ColorLists::dropMoving(QWidget * source,const QMimeData * mime,QPoint pos)
{
  if (source==this) return false ;
  return true                    ;
}

bool N::ColorLists::dropAppend(QWidget * source,const QMimeData * mime,QPoint pos)
{
  if (source==this) return false           ;
  return dropItems ( source , mime , pos ) ;
}

QString N::ColorLists::MimeType(const QMimeData * mime)
{
  return AbstractGui::MimeType(mime,"picture/uuid;picture/uuids") ;
}

UUIDs N::ColorLists::MimeUuids(const QMimeData * mime,QString mimetype)
{
  UUIDs Uuids                            ;
  QByteArray data = mime->data(mimetype) ;
  if (data.size()<=0) return Uuids       ;
  if (mimetype=="picture/uuid")          {
    SUID * suid = (SUID *)data.data()    ;
    Uuids << (*suid)                     ;
  } else
  if (mimetype=="picture/uuids")         {
    SUID * suid  = (SUID *)data.data()   ;
    int    total = (int)suid[0]          ;
    for (int i=0;i<total;i++)            {
      Uuids << suid[i+1]                 ;
      plan->processEvents()              ;
    }                                    ;
  }                                      ;
  return Uuids                           ;
}

QString N::ColorLists::DS(double v)
{
  return QString("%1").arg(v,0,'f',4) ;
}

void N::ColorLists::setRgbaHeader(void)
{
  NewTreeWidgetItem        ( head                ) ;
  setColumnCount           ( 6                   ) ;
  head -> setText          ( 0 , ""              ) ;
  head -> setText          ( 1 , tr("Red"  )     ) ;
  head -> setText          ( 2 , tr("Green")     ) ;
  head -> setText          ( 3 , tr("Blue" )     ) ;
  head -> setText          ( 4 , tr("Alpha")     ) ;
  head -> setText          ( 5 , ""              ) ;
  for (int i=1;i<columnCount();i++)                {
    setAlignment ( i , Qt::AlignRight              |
                       Qt::AlignVCenter          ) ;
  }                                                ;
  assignHeaderItems        ( head                ) ;
}

void N::ColorLists::setCmykHeader(void)
{
  NewTreeWidgetItem        ( head                ) ;
  setColumnCount           ( 6                   ) ;
  head -> setText          ( 0 , ""              ) ;
  head -> setText          ( 1 , tr("Cyan"   )   ) ;
  head -> setText          ( 2 , tr("Magenta")   ) ;
  head -> setText          ( 3 , tr("Yellow" )   ) ;
  head -> setText          ( 4 , tr("Black"  )   ) ;
  head -> setText          ( 5 , ""              ) ;
  for (int i=1;i<columnCount();i++)                {
    setAlignment ( i , Qt::AlignRight              |
                       Qt::AlignVCenter          ) ;
  }                                                ;
  assignHeaderItems        ( head                ) ;
}

void N::ColorLists::setHsvHeader(void)
{
  NewTreeWidgetItem        ( head                ) ;
  setColumnCount           ( 6                   ) ;
  head -> setText          ( 0 , ""              ) ;
  head -> setText          ( 1 , tr("Hue"       )) ;
  head -> setText          ( 2 , tr("Saturation")) ;
  head -> setText          ( 3 , tr("Value"     )) ;
  head -> setText          ( 4 , tr("Alpha"     )) ;
  head -> setText          ( 5 , ""              ) ;
  for (int i=1;i<columnCount();i++)                {
    setAlignment ( i , Qt::AlignRight              |
                       Qt::AlignVCenter          ) ;
  }                                                ;
  assignHeaderItems        ( head                ) ;
}

void N::ColorLists::setHslHeader(void)
{
  NewTreeWidgetItem        ( head                ) ;
  setColumnCount           ( 6                   ) ;
  head -> setText          ( 0 , ""              ) ;
  head -> setText          ( 1 , tr("Hue"       )) ;
  head -> setText          ( 2 , tr("Saturation")) ;
  head -> setText          ( 3 , tr("Lightness" )) ;
  head -> setText          ( 4 , tr("Alpha"     )) ;
  head -> setText          ( 5 , ""              ) ;
  for (int i=1;i<columnCount();i++)                {
    setAlignment ( i , Qt::AlignRight              |
                       Qt::AlignVCenter          ) ;
  }                                                ;
  assignHeaderItems        ( head                ) ;
}

void N::ColorLists::setYuvHeader(void)
{
  NewTreeWidgetItem        ( head                ) ;
  setColumnCount           ( 5                   ) ;
  head -> setText          ( 0 , ""              ) ;
  head -> setText          ( 1 , tr("Luminance") ) ;
  head -> setText          ( 2 , tr("U"        ) ) ;
  head -> setText          ( 3 , tr("V"        ) ) ;
  head -> setText          ( 4 , ""              ) ;
  for (int i=1;i<columnCount();i++)                {
    setAlignment ( i , Qt::AlignRight              |
                       Qt::AlignVCenter          ) ;
  }                                                ;
  assignHeaderItems        ( head                ) ;
}

void N::ColorLists::setYiqHeader(void)
{
  NewTreeWidgetItem        ( head                ) ;
  setColumnCount           ( 5                   ) ;
  head -> setText          ( 0 , ""              ) ;
  head -> setText          ( 1 , tr("Luminance" )) ;
  head -> setText          ( 2 , tr("In-phase"  )) ;
  head -> setText          ( 3 , tr("Quadrature")) ;
  head -> setText          ( 4 , ""              ) ;
  for (int i=1;i<columnCount();i++)                {
    setAlignment ( i , Qt::AlignRight              |
                       Qt::AlignVCenter          ) ;
  }                                                ;
  assignHeaderItems        ( head                ) ;
}

void N::ColorLists::setXyzHeader(void)
{
  NewTreeWidgetItem        ( head                ) ;
  setColumnCount           ( 5                   ) ;
  head -> setText          ( 0 , ""              ) ;
  head -> setText          ( 1 , tr("X")         ) ;
  head -> setText          ( 2 , tr("Y")         ) ;
  head -> setText          ( 3 , tr("Z")         ) ;
  head -> setText          ( 4 , ""              ) ;
  for (int i=1;i<columnCount();i++)                {
    setAlignment ( i , Qt::AlignRight              |
                       Qt::AlignVCenter          ) ;
  }                                                ;
  assignHeaderItems        ( head                ) ;
}

void N::ColorLists::setLabHeader(void)
{
  NewTreeWidgetItem        ( head                ) ;
  setColumnCount           ( 5                   ) ;
  head -> setText          ( 0 , ""              ) ;
  head -> setText          ( 1 , tr("Lightness") ) ;
  head -> setText          ( 2 , tr("a"        ) ) ;
  head -> setText          ( 3 , tr("b"        ) ) ;
  head -> setText          ( 4 , ""              ) ;
  for (int i=1;i<columnCount();i++)                {
    setAlignment ( i , Qt::AlignRight              |
                       Qt::AlignVCenter          ) ;
  }                                                ;
  assignHeaderItems        ( head                ) ;
}

void N::ColorLists::setLuvHeader(void)
{
  NewTreeWidgetItem        ( head                ) ;
  setColumnCount           ( 5                   ) ;
  head -> setText          ( 0 , ""              ) ;
  head -> setText          ( 1 , tr("Lightness") ) ;
  head -> setText          ( 2 , tr("U"        ) ) ;
  head -> setText          ( 3 , tr("V"        ) ) ;
  head -> setText          ( 4 , ""              ) ;
  for (int i=1;i<columnCount();i++)                {
    setAlignment ( i , Qt::AlignRight              |
                       Qt::AlignVCenter          ) ;
  }                                                ;
  assignHeaderItems        ( head                ) ;
}

void N::ColorLists::setColorHeader(void)
{
  switch (model)                  {
    case Graphics::RGBA           :
      setRgbaHeader ( )           ;
    break                         ;
    case Graphics::CMYK           :
      setCmykHeader ( )           ;
    break                         ;
    case Graphics::HSV            :
      setHsvHeader  ( )           ;
    break                         ;
    case Graphics::HSL            :
      setHslHeader  ( )           ;
    break                         ;
    case Graphics::YUV            :
      setYuvHeader  ( )           ;
    break                         ;
    case Graphics::YIQ            :
      setYiqHeader  ( )           ;
    break                         ;
    case Graphics::XYZ            :
      setXyzHeader  ( )           ;
    break                         ;
    case Graphics::Lab            :
      setLabHeader  ( )           ;
    break                         ;
    case Graphics::LUV            :
      setLuvHeader  ( )           ;
    break                         ;
  }                               ;
}

void N::ColorLists::ListRgba(QTreeWidgetItem * it,QRgb rgb)
{
  it -> setText ( 1 , QString::number(qRed  (rgb)) ) ;
  it -> setText ( 2 , QString::number(qGreen(rgb)) ) ;
  it -> setText ( 3 , QString::number(qBlue (rgb)) ) ;
  it -> setText ( 4 , QString::number(qAlpha(rgb)) ) ;
  setAlignments ( it                               ) ;
}

void N::ColorLists::ListCmyk(QTreeWidgetItem * it,QRgb rgb)
{
  QColor c(rgb)                                      ;
  it -> setText ( 1 , QString::number(c.cyan   ()) ) ;
  it -> setText ( 2 , QString::number(c.magenta()) ) ;
  it -> setText ( 3 , QString::number(c.yellow ()) ) ;
  it -> setText ( 4 , QString::number(c.black  ()) ) ;
  setAlignments ( it                               ) ;
}

void N::ColorLists::ListHsv(QTreeWidgetItem * it,QRgb rgb)
{
  QColor c(rgb)                                            ;
  it -> setText ( 1 , QString::number(c.hsvHue       ()) ) ;
  it -> setText ( 2 , QString::number(c.hsvSaturation()) ) ;
  it -> setText ( 3 , QString::number(c.value        ()) ) ;
  it -> setText ( 4 , QString::number(c.alpha        ()) ) ;
  setAlignments ( it                                     ) ;
}

void N::ColorLists::ListHsl(QTreeWidgetItem * it,QRgb rgb)
{
  QColor c(rgb)                                            ;
  it -> setText ( 1 , QString::number(c.hslHue       ()) ) ;
  it -> setText ( 2 , QString::number(c.hslSaturation()) ) ;
  it -> setText ( 3 , QString::number(c.lightness    ()) ) ;
  it -> setText ( 4 , QString::number(c.alpha        ()) ) ;
  setAlignments ( it                                     ) ;
}

void N::ColorLists::ListYuv(QTreeWidgetItem * it,QRgb rgb)
{
  QColor c(rgb)                   ;
  ColorYUV YUV(c)                 ;
  it -> setText ( 1 , DS(YUV.Y) ) ;
  it -> setText ( 2 , DS(YUV.U) ) ;
  it -> setText ( 3 , DS(YUV.V) ) ;
  setAlignments ( it            ) ;
}

void N::ColorLists::ListYiq(QTreeWidgetItem * it,QRgb rgb)
{
  QColor c(rgb)                   ;
  ColorYIQ YIQ(c)                 ;
  it -> setText ( 1 , DS(YIQ.Y) ) ;
  it -> setText ( 2 , DS(YIQ.I) ) ;
  it -> setText ( 3 , DS(YIQ.Q) ) ;
  setAlignments ( it            ) ;
}

void N::ColorLists::ListXyz(QTreeWidgetItem * it,QRgb rgb)
{
  QColor c(rgb)                   ;
  ColorXYZ XYZ(c)                 ;
  it -> setText ( 1 , DS(XYZ.X) ) ;
  it -> setText ( 2 , DS(XYZ.Y) ) ;
  it -> setText ( 3 , DS(XYZ.Z) ) ;
  setAlignments ( it            ) ;
}

void N::ColorLists::ListLab(QTreeWidgetItem * it,QRgb rgb)
{
  QColor c(rgb)                   ;
  ColorLab Lab(c)                 ;
  it -> setText ( 1 , DS(Lab.L) ) ;
  it -> setText ( 2 , DS(Lab.a) ) ;
  it -> setText ( 3 , DS(Lab.b) ) ;
  setAlignments ( it            ) ;
}

void N::ColorLists::ListLuv(QTreeWidgetItem * it,QRgb rgb)
{
  QColor c(rgb)                   ;
  ColorLUV LUV(c)                 ;
  it -> setText ( 1 , DS(LUV.L) ) ;
  it -> setText ( 2 , DS(LUV.U) ) ;
  it -> setText ( 3 , DS(LUV.V) ) ;
  setAlignments ( it            ) ;
}

void N::ColorLists::run(int T,ThreadData * d)
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

void N::ColorLists::Load(ThreadData * d)
{
  nDropOut     ( ! IsContinue ( d )  )        ;
  nDropOut     ( ObjectUuid ( ) <= 0 )        ;
  colors . clear ( )                          ;
  PictureManager PM ( plan )                  ;
  EnterSQL     ( SC , plan -> sql    )        ;
    switch     ( ObjectType ( )      )        {
      case Types::Palette                     :
        if ( ! isStandby ( ) )                {
          Palette palette                     ;
          if (PM.GetPalette(SC,uuid,palette)) {
            palette.toColors(colors)          ;
          }                                   ;
        }                                     ;
      break                                   ;
      case Types::ColorGroup                  :
        if ( ! isStandby ( ) )                {
          LoadColors ( SC , uuid , colors )   ;
        }                                     ;
      break                                   ;
    }                                         ;
  LeaveSQL     ( SC , plan->sql      )        ;
  emit Refresh (                     )        ;
}

bool N::ColorLists::startup(void)
{
  start ( 10001 ) ;
  return true     ;
}

void N::ColorLists::ColorIcon(QTreeWidgetItem * it,QRgb rgb,QSize isize)
{
  QImage W(isize,QImage::Format_ARGB32)       ;
  QRect  R(0,0,isize.width(),isize.height())  ;
  memset(W.bits(),0,W.byteCount())            ;
  QPainter p                                  ;
  p . begin     ( &W          )               ;
  p . setPen    ( QColor(rgb) )               ;
  p . setBrush  ( QColor(rgb) )               ;
  p . drawRect  ( R           )               ;
  p . end       (             )               ;
  it->setIcon(0,QIcon(QPixmap::fromImage(W))) ;
}

void N::ColorLists::List(void)
{
  QSize IC(24,24)                       ;
  QHeaderView * hv = header()           ;
  if (NotNull(hv))                      {
    int h = hv->height()                ;
    IC = QSize(h,h)                     ;
  }                                     ;
  int t = colors.count()                ;
  clear           (                   ) ;
  setColorHeader  (                   ) ;
  for (int i=0;i<t;i++)                 {
    QRgb rgb = (QRgb)colors[i]          ;
    NewTreeWidgetItem ( it            ) ;
    switch (model)                      {
      case Graphics::RGBA               :
        ListRgba ( it , rgb )           ;
      break                             ;
      case Graphics::CMYK               :
        ListCmyk ( it , rgb )           ;
      break                             ;
      case Graphics::HSV                :
        ListHsv  ( it , rgb )           ;
      break                             ;
      case Graphics::HSL                :
        ListHsl  ( it , rgb )           ;
      break                             ;
      case Graphics::YUV                :
        ListYuv  ( it , rgb )           ;
      break                             ;
      case Graphics::YIQ                :
        ListYiq  ( it , rgb )           ;
      break                             ;
      case Graphics::XYZ                :
        ListXyz  ( it , rgb )           ;
      break                             ;
      case Graphics::Lab                :
        ListLab  ( it , rgb )           ;
      break                             ;
      case Graphics::LUV                :
        ListLuv  ( it , rgb )           ;
      break                             ;
    }                                   ;
    ColorIcon         ( it , rgb , IC ) ;
    addTopLevelItem   ( it            ) ;
    it->setData   (0,Qt::UserRole,rgb ) ;
  }                                     ;
  SuitableColumns (                   ) ;
  reportItems     (                   ) ;
  Alert           ( Done              ) ;
}

bool N::ColorLists::dropColor(QWidget * source,QPointF pos,const QColor & color)
{ Q_UNUSED ( source ) ;
  Q_UNUSED ( pos    ) ;
  Q_UNUSED ( color  ) ;
    Notify("dropColor") ;
  return true ;
}

bool N::ColorLists::dropImage(QWidget * source,QPointF pos,const QImage & image)
{ Q_UNUSED ( source ) ;
  Q_UNUSED ( pos    ) ;
  Q_UNUSED ( image  ) ;
    Notify("dropImage") ;
  return true ;
}

bool N::ColorLists::dropPictures(QWidget * source,QPointF pos,const UUIDs & Uuids)
{ Q_UNUSED ( source ) ;
  Q_UNUSED ( pos    ) ;
  Q_UNUSED ( Uuids  ) ;
    Notify("dropPictures") ;
  return true ;
}

void N::ColorLists::Insert(void)
{
  QRgb              rgb                                                 ;
  QTreeWidgetItem * item = currentItem ()                               ;
  if (NotNull(item))                                                    {
    rgb = (QRgb)item->data(0,Qt::UserRole).toUInt()                     ;
  }                                                                     ;
  QColor c(rgb)                                                         ;
  c = QColorDialog::getColor(c,this,tr("Add color"))                    ;
  rgb = c.rgba()                                                        ;
  unsigned int v = (unsigned int)rgb                                    ;
  if (colors.contains(v))                                               {
    Alert ( Error )                                                     ;
    return                                                              ;
  }                                                                     ;
  nDropOut ( colors.contains(v) )                                       ;
  if (v==0)                                                             {
    if (QMessageBox::question                                           (
                this                                                    ,
                tr("Black color")                                       ,
                tr("Are you sure you want to add black into this group?")
                ) != QMessageBox::Ok )                                  {
      Alert ( Error )                                                   ;
      return                                                            ;
    }                                                                   ;
  }                                                                     ;
  colors << v                                                           ;
  Graphics::Uniqueness(colors)                                ;
  List ( )                                                              ;
}

void N::ColorLists::Delete(void)
{
  QTreeWidgetItem * item = currentItem ()              ;
  if (IsNull(item))                                    {
    Alert ( Error )                                    ;
    return                                             ;
  }                                                    ;
  QRgb rgb = (QRgb)item->data(0,Qt::UserRole).toUInt() ;
  unsigned int v = (unsigned int)rgb                   ;
  int index = colors.indexOf(v)                        ;
  if (index<0)                                         {
    Alert ( Error )                                    ;
    return                                             ;
  }                                                    ;
  colors.takeAt(index)                                 ;
  index = indexOfTopLevelItem(item)                    ;
  if (index>=0) takeTopLevelItem(index)                ;
  Alert ( Done )                                       ;
}

void N::ColorLists::Export(void)
{
  QString     filename                             ;
  filename = QFileDialog::getSaveFileName          (
               this                                ,
               tr("Export color values into file") ,
               plan->Temporary("")                 ,
               tr("Plain text (*.txt)\n"
                  "Any kinds (*.*)")             ) ;
  if (filename.length()<=0) return                 ;
  QStringList L                                    ;
  QStringList M                                    ;
  QString     B                                    ;
  QByteArray  V                                    ;
  int         c = columnCount() - 1                ;
  for (int i=0;i<topLevelItemCount();i++)          {
    QTreeWidgetItem * it = topLevelItem(i)         ;
    M . clear ( )                                  ;
    for (int x=1;x<c;x++) M << it->text(x)         ;
    L << M.join("\t")                              ;
  }                                                ;
  nDropOut ( L.count() <= 0 )                      ;
  B = L.join("\n")                                 ;
  V = B.toUtf8()                                   ;
  File::toFile(filename,V)                         ;
  Alert ( Done )                                   ;
}

void N::ColorLists::Save(void)
{
  nDropOut   ( uuid <= 0          )         ;
  PictureManager PM ( plan )                ;
  EnterSQL   ( SC , plan->sql     )         ;
    Palette palette                         ;
    switch (type)                           {
      case Types::Palette                   :
        if (!isStandby())                   {
          palette.set(colors)               ;
          PM.assurePalette(SC,uuid,palette) ;
        }                                   ;
      break                                 ;
      case Types::ColorGroup                :
        if (!isStandby())                   {
          SaveColors ( SC , uuid , colors ) ;
        }                                   ;
      break                                 ;
    }                                       ;
  LeaveSQL   ( SC , plan->sql     )         ;
  Alert      ( Done               )         ;
}

bool N::ColorLists::Menu(QPoint pos)
{
  MenuManager       mm(this)                                     ;
  QTreeWidgetItem * item = itemAt(pos)                           ;
  QAction         * aa   = NULL                                  ;
  QMenu           * mc   = NULL                                  ;
  QMenu           * me   = NULL                                  ;
  me = mm.addMenu(tr("Edit"        ))                            ;
  mm . add(me, 101,tr("Reload")                                ) ;
  mm . add(me, 102,tr("Insert")                                ) ;
  if (NotNull(item))                                             {
    mm . addSeparator ( me )                                     ;
    mm . add(me, 201,tr("Delete")                              ) ;
  }                                                              ;
  mm . addSeparator ( me )                                       ;
  mm . add(me, 301,tr("Save"  )                                ) ;
  mm . add(me, 302,tr("Export")                                ) ;
  mc = mm.addMenu(tr("Color models"))                            ;
  mm . add(mc,1001,tr("RGBA"),true,nEqual(model,Graphics::RGBA)) ;
  mm . add(mc,1002,tr("CMYK"),true,nEqual(model,Graphics::CMYK)) ;
  mm . add(mc,1003,tr("HSV" ),true,nEqual(model,Graphics::HSV )) ;
  mm . add(mc,1004,tr("HSL" ),true,nEqual(model,Graphics::HSL )) ;
  mm . add(mc,1005,tr("YUV" ),true,nEqual(model,Graphics::YUV )) ;
  mm . add(mc,1006,tr("YIQ" ),true,nEqual(model,Graphics::YIQ )) ;
  mm . add(mc,1007,tr("XYZ" ),true,nEqual(model,Graphics::XYZ )) ;
  mm . add(mc,1008,tr("Lab" ),true,nEqual(model,Graphics::Lab )) ;
  mm . add(mc,1009,tr("LUV" ),true,nEqual(model,Graphics::LUV )) ;
  ////////////////////////////////////////////////////////////////
  DockingMenu    ( mm   )                                        ;
  ////////////////////////////////////////////////////////////////
  mm . setFont   ( plan )                                        ;
  aa = mm . exec (      )                                        ;
  nKickOut ( IsNull(aa) , true )                                 ;
  ////////////////////////////////////////////////////////////////
  if ( RunDocking ( mm , aa ) ) return true                      ;
  ////////////////////////////////////////////////////////////////
  switch ( mm [ aa ] )                                           {
    case 101                                                     :
      startup ( )                                                ;
    break                                                        ;
    case 102                                                     :
      Insert  ( )                                                ;
    break                                                        ;
    case 201                                                     :
      Delete  ( )                                                ;
    break                                                        ;
    case 301                                                     :
      Save    ( )                                                ;
    break                                                        ;
    case 302                                                     :
      Export  ( )                                                ;
    break                                                        ;
    case 1001                                                    :
      if (aa->isChecked())                                       {
        model = Graphics::RGBA                                   ;
        List ( )                                                 ;
      }                                                          ;
    break                                                        ;
    case 1002                                                    :
      if (aa->isChecked())                                       {
        model = Graphics::CMYK                                   ;
        List ( )                                                 ;
      }                                                          ;
    break                                                        ;
    case 1003                                                    :
      if (aa->isChecked())                                       {
        model = Graphics::HSV                                    ;
        List ( )                                                 ;
      }                                                          ;
    break                                                        ;
    case 1004                                                    :
      if (aa->isChecked())                                       {
        model = Graphics::HSL                                    ;
        List ( )                                                 ;
      }                                                          ;
    break                                                        ;
    case 1005                                                    :
      if (aa->isChecked())                                       {
        model = Graphics::YUV                                    ;
        List ( )                                                 ;
      }                                                          ;
    break                                                        ;
    case 1006                                                    :
      if (aa->isChecked())                                       {
        model = Graphics::YIQ                                    ;
        List ( )                                                 ;
      }                                                          ;
    break                                                        ;
    case 1007                                                    :
      if (aa->isChecked())                                       {
        model = Graphics::XYZ                                    ;
        List ( )                                                 ;
      }                                                          ;
    break                                                        ;
    case 1008                                                    :
      if (aa->isChecked())                                       {
        model = Graphics::Lab                                    ;
        List ( )                                                 ;
      }                                                          ;
    break                                                        ;
    case 1009                                                    :
      if (aa->isChecked())                                       {
        model = Graphics::LUV                                    ;
        List ( )                                                 ;
      }                                                          ;
    break                                                        ;
  }                                                              ;
  return true                                                    ;
}
