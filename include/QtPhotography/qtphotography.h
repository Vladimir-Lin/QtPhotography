/****************************************************************************
 *                                                                          *
 * Copyright (C) 2001 ~ 2016 Neutrino International Inc.                    *
 *                                                                          *
 * Author : Brian Lin <lin.foxman@gmail.com>, Skype: wolfram_lin            *
 *                                                                          *
 ****************************************************************************/

#ifndef QT_PHOTOGRAPHY_H
#define QT_PHOTOGRAPHY_H

#include <QtVCF>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#    if defined(QT_BUILD_QTPHOTOGRAPHY_LIB)
#      define Q_PHOTOGRAPHY_EXPORT Q_DECL_EXPORT
#    else
#      define Q_PHOTOGRAPHY_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define Q_PHOTOGRAPHY_EXPORT
#endif


namespace Ui                                  {
class Q_PHOTOGRAPHY_EXPORT nPictureComparsion ;
class Q_PHOTOGRAPHY_EXPORT GalleryOperator    ;
class Q_PHOTOGRAPHY_EXPORT GalleryByHost      ;
}

namespace N
{

class Q_PHOTOGRAPHY_EXPORT ColorGroups        ;
class Q_PHOTOGRAPHY_EXPORT ColorLists         ;
class Q_PHOTOGRAPHY_EXPORT PaletteLists       ;
class Q_PHOTOGRAPHY_EXPORT ShapeLists         ;
class Q_PHOTOGRAPHY_EXPORT ContourLists       ;
class Q_PHOTOGRAPHY_EXPORT PictureComparsion  ;
class Q_PHOTOGRAPHY_EXPORT ImageSupports      ;
class Q_PHOTOGRAPHY_EXPORT GalleryView        ;
class Q_PHOTOGRAPHY_EXPORT AttachGallery      ;
class Q_PHOTOGRAPHY_EXPORT PictureView        ;
class Q_PHOTOGRAPHY_EXPORT ObjectGallery      ;
class Q_PHOTOGRAPHY_EXPORT DropPictures       ;
class Q_PHOTOGRAPHY_EXPORT PictureDepot       ;
class Q_PHOTOGRAPHY_EXPORT GalleryOperator    ;
class Q_PHOTOGRAPHY_EXPORT GalleryByHost      ;
class Q_PHOTOGRAPHY_EXPORT FeatureExtractor   ;
class Q_PHOTOGRAPHY_EXPORT PictureMatcher     ;
class Q_PHOTOGRAPHY_EXPORT PicturesDispatcher ;
class Q_PHOTOGRAPHY_EXPORT PictureManipulator ;
class Q_PHOTOGRAPHY_EXPORT GalleryShelf       ;
class Q_PHOTOGRAPHY_EXPORT PictureShelf       ;
class Q_PHOTOGRAPHY_EXPORT SubgroupShelf      ;

namespace Images
{

Q_PHOTOGRAPHY_EXPORT bool Embedded    (QImage & input,QImage & output,QByteArray & body) ;
Q_PHOTOGRAPHY_EXPORT bool Extract     (QImage & input,QByteArray & body) ;
Q_PHOTOGRAPHY_EXPORT int  DecryptPair (QByteArray & Body,SUID & signature) ;
Q_PHOTOGRAPHY_EXPORT bool EncryptPair (SUID signature,QByteArray & Source,QByteArray & Encrypted,QByteArray & Keys) ;
Q_PHOTOGRAPHY_EXPORT bool DecryptBody (SUID signature,QByteArray & Source,QByteArray & Encrypted,QByteArray & Keys) ;

}

/*****************************************************************************
 *                                                                           *
 *                               Graphics Widgets                            *
 *                                                                           *
 *****************************************************************************/

class Q_PHOTOGRAPHY_EXPORT ColorGroups : public TreeDock
{
  Q_OBJECT
  public:

    explicit ColorGroups           (StandardConstructor) ;
    virtual ~ColorGroups           (void) ;

  protected:

    QPoint dragPoint ;

    virtual bool FocusIn           (void) ;
    virtual void Configure         (void) ;

    virtual bool        hasItem    (void) ;
    virtual bool        startDrag  (QMouseEvent * event) ;
    virtual bool        fetchDrag  (QMouseEvent * event) ;
    virtual QMimeData * dragMime   (void) ;
    virtual void        dragDone   (Qt::DropAction dropIt,QMimeData * mime) ;
    virtual bool        finishDrag (QMouseEvent * event) ;

    virtual bool        acceptDrop (QWidget * source,const QMimeData * mime);
    virtual bool        dropNew    (QWidget * source,const QMimeData * mime,QPoint pos);
    virtual bool        dropMoving (QWidget * source,const QMimeData * mime,QPoint pos);
    virtual bool        dropAppend (QWidget * source,const QMimeData * mime,QPoint pos);

    virtual QString     MimeType   (const QMimeData * mime);
    virtual UUIDs       MimeUuids  (const QMimeData * mime,QString mimetype) ;

    virtual void        run        (int Type,ThreadData * data) ;
    virtual void        Load       (ThreadData * data) ;

  private:

  public slots:

    virtual bool startup           (void) ;
    virtual void Insert            (void) ;

  protected slots:

    virtual bool Menu              (QPoint pos) ;

    virtual void singleClicked     (QTreeWidgetItem * item,int column) ;
    virtual void doubleClicked     (QTreeWidgetItem * item,int column) ;
    virtual void nameFinished      (void) ;

  private slots:

  signals:

    void ValueList  (QString name,SUID uuid) ;
    void ShowColors (QString name,SUID uuid) ;

} ;

class Q_PHOTOGRAPHY_EXPORT ColorLists : public TreeDock
                                      , public Object
                                      , public GraphicsManager
{
  Q_OBJECT
  public:

    Colors               colors ;
    Graphics::ColorModel model  ;

    explicit            ColorLists     (StandardConstructor) ;
    virtual            ~ColorLists     (void) ;

    virtual QSize       sizeHint       (void) const ;

  protected:

    QPoint dragPoint ;

    virtual bool        FocusIn        (void) ;
    virtual void        Configure      (void) ;

    virtual bool        hasItem        (void) ;
    virtual bool        startDrag      (QMouseEvent * event) ;
    virtual bool        fetchDrag      (QMouseEvent * event) ;
    virtual QMimeData * dragMime       (void) ;
    virtual void        dragDone       (Qt::DropAction dropIt,QMimeData * mime) ;
    virtual bool        finishDrag     (QMouseEvent * event) ;

    virtual bool        acceptDrop     (QWidget * source,const QMimeData * mime);
    virtual bool        dropNew        (QWidget * source,const QMimeData * mime,QPoint pos);
    virtual bool        dropMoving     (QWidget * source,const QMimeData * mime,QPoint pos);
    virtual bool        dropAppend     (QWidget * source,const QMimeData * mime,QPoint pos);

    virtual QString     MimeType       (const QMimeData * mime);
    virtual UUIDs       MimeUuids      (const QMimeData * mime,QString mimetype) ;

    virtual bool        dropColor      (nDeclDrops,const QColor  & color) ;
    virtual bool        dropImage      (nDeclDrops,const QImage  & image) ;
    virtual bool        dropPictures   (nDeclDrops,const UUIDs   & Uuids) ;

    virtual void        run            (int Type,ThreadData * data) ;
    virtual void        Load           (ThreadData * data) ;

  private:

    void                ColorIcon      (QTreeWidgetItem * it,QRgb rgb,QSize iconSize) ;

    void                ListRgba       (QTreeWidgetItem * it,QRgb rgb) ;
    void                ListCmyk       (QTreeWidgetItem * it,QRgb rgb) ;
    void                ListHsv        (QTreeWidgetItem * it,QRgb rgb) ;
    void                ListHsl        (QTreeWidgetItem * it,QRgb rgb) ;
    void                ListYuv        (QTreeWidgetItem * it,QRgb rgb) ;
    void                ListYiq        (QTreeWidgetItem * it,QRgb rgb) ;
    void                ListXyz        (QTreeWidgetItem * it,QRgb rgb) ;
    void                ListLab        (QTreeWidgetItem * it,QRgb rgb) ;
    void                ListLuv        (QTreeWidgetItem * it,QRgb rgb) ;

    void                setRgbaHeader  (void) ;
    void                setCmykHeader  (void) ;
    void                setHsvHeader   (void) ;
    void                setHslHeader   (void) ;
    void                setYuvHeader   (void) ;
    void                setYiqHeader   (void) ;
    void                setXyzHeader   (void) ;
    void                setLabHeader   (void) ;
    void                setLuvHeader   (void) ;

    QString             DS             (double v) ;

  public slots:

    virtual bool        startup        (void) ;
    virtual void        Delete         (void) ;
    virtual void        Export         (void) ;
    virtual void        Save           (void) ;
    virtual void        List           (void) ;
    virtual void        Insert         (void) ;
    void                setColorHeader (void) ;

  protected slots:

    virtual bool        Menu           (QPoint pos) ;

  private slots:

  signals:

    void                Refresh        (void) ;

} ;

class Q_PHOTOGRAPHY_EXPORT PaletteLists : public ListDock
                                        , public Ownership
                                        , public PictureManager
{
  Q_OBJECT
  public:

    explicit PaletteLists       (StandardConstructor) ;
    virtual ~PaletteLists       (void) ;

    virtual QSize sizeHint      (void) const ;

  protected:

    virtual bool FocusIn        (void) ;
    virtual void Configure      (void) ;

    int          Count          (SqlConnection & Connection) ;
    UUIDs        GetUuids       (SqlConnection & Connection) ;

    virtual void run            (int Type,ThreadData * Data) ;
    virtual void Loading        (ThreadData * data) ;

  private:

  public slots:

    virtual bool startup        (void) ;

    virtual void Rename         (void) ;
    virtual void Rename         (QListWidgetItem * item) ;

  protected slots:

    virtual bool Menu           (QPoint pos) ;

    virtual void renameFinished (void) ;

  private slots:

  signals:

    void ValueList              (QString name,SUID uuid) ;

} ;

class Q_PHOTOGRAPHY_EXPORT ShapeLists : public ItemEditor
                                      , public GraphicsManager
{
  Q_OBJECT
  public:

    explicit ShapeLists    (StandardConstructor) ;
    virtual ~ShapeLists    (void) ;

  protected:

    virtual void Configure (void) ;
    virtual bool FocusIn   (void) ;

    // Drag functions

  private:

  public slots:

    virtual void Insert    (void) ;
    virtual void Delete    (void) ;
    virtual void Copy      (void) ;

  protected slots:

  private slots:

  signals:

} ;

class Q_PHOTOGRAPHY_EXPORT ContourLists : public ListDock
{
  Q_OBJECT
  public:

    explicit ContourLists         (StandardConstructor) ;
    virtual ~ContourLists         (void) ;

    virtual QSize sizeHint        (void) const ;

  protected:

    QPoint dragPoint ;

    virtual void Configure         (void) ;
    virtual bool FocusIn           (void) ;

    virtual bool        hasItem    (void);
    virtual bool        startDrag  (QMouseEvent * event);
    virtual bool        fetchDrag  (QMouseEvent * event);
    virtual QMimeData * dragMime   (void);
    virtual void        dragDone   (Qt::DropAction dropIt,QMimeData * mime);
    virtual bool        finishDrag (QMouseEvent * event);

    virtual void run               (int Type,ThreadData * data) ;
    virtual bool Load              (ThreadData * data) ;

  private:

  public slots:

    virtual bool startup           (void) ;
    virtual void List              (void) ;
    virtual void Copy              (void) ;

  protected slots:

    bool         Menu              (QPoint pos) ;

  private slots:

  signals:

    void         Append            (QWidget * widget) ;

} ;

class Q_PHOTOGRAPHY_EXPORT PictureComparsion : public Widget
{
  Q_OBJECT
  public:

    explicit PictureComparsion  (StandardConstructor) ;
    virtual ~PictureComparsion  (void) ;

  protected:

    Ui::nPictureComparsion * ui     ;
    UUIDs                    Uuids  ;
    QList<QImage>            Images ;

    virtual void Configure      (void) ;

    virtual void focusInEvent   (QFocusEvent  * event) ;
    virtual void focusOutEvent  (QFocusEvent  * event) ;
    virtual void resizeEvent    (QResizeEvent * event) ;
    virtual void showEvent      (QShowEvent   * event) ;

    virtual void dragEnterEvent (QDragEnterEvent * event) ;
    virtual void dragLeaveEvent (QDragLeaveEvent * event) ;
    virtual void dragMoveEvent  (QDragMoveEvent  * event) ;
    virtual void dropEvent      (QDropEvent      * event) ;

    virtual bool acceptDrop     (QWidget * source,const QMimeData * mime);
    virtual bool dropNew        (QWidget * source,const QMimeData * mime,QPoint pos);
    virtual bool dropMoving     (QWidget * source,const QMimeData * mime,QPoint pos);
    virtual bool dropAppend     (QWidget * source,const QMimeData * mime,QPoint pos);

    virtual bool dropPictures   (nDeclDrops,const UUIDs & Uuids) ;

  private:

  public slots:

    virtual bool Relocation     (void) ;
    virtual bool Compare        (void) ;

  protected slots:

    virtual void Options        (int index) ;

  private slots:

  signals:

} ;

class Q_PHOTOGRAPHY_EXPORT ImageSupports : public TreeWidget
{
  Q_OBJECT
  public:

    explicit ImageSupports (StandardConstructor) ;
    virtual ~ImageSupports (void) ;

    virtual QSize sizeHint (void) const ;

  protected:

    virtual void Configure (void) ;

  private:

  public slots:

    virtual void List      (void) ;

  protected slots:

  private slots:

  signals:

} ;

class Q_PHOTOGRAPHY_EXPORT GalleryView : public GroupView
{
  Q_OBJECT
  public:

    explicit GalleryView            (StandardConstructor) ;
    virtual ~GalleryView            (void) ;

  protected:

    virtual void    Configure       (void) ;

    virtual QString MimeType        (const QMimeData * mime);
    virtual UUIDs   MimeUuids       (const QMimeData * mime,QString mimetype) ;

    virtual QString ToolTipTotoal   (int total) ;
    virtual int     Counting        (SqlConnection & Connection,SUID uuid) ;

  private:

  public slots:

    virtual void    DeletePictures  (QListWidgetItem * it) ;

    virtual void    GroupPositions  (RMAPs & Position,UUIDs & Uuids) ;
    virtual bool    JoinUrls        (SUID uuid,const QList<QUrl> & urls) ;
    virtual bool    JoinImage       (SUID uuid,const QImage & image) ;
    virtual bool    JoinPictures    (SUID uuid,const UUIDs & Uuids) ;

  protected slots:

    virtual bool    Menu            (QPoint pos) ;

  private slots:

  signals:

    void            Constraints     (QString name,SUID uuid) ;
    void            RuleTables      (QString name,SUID uuid) ;

} ;

class Q_PHOTOGRAPHY_EXPORT AttachGallery : public GalleryView
{
  Q_OBJECT
  public:

    SUID AttachUuid ;
    int  AttachType ;

    explicit AttachGallery    (StandardConstructor);
    virtual ~AttachGallery    (void);

  protected:

    virtual UUIDs GroupUuids  (SqlConnection & Connection) ;
    virtual SUID  AppendGroup (SqlConnection & Connection,QString name) ;

  private:

  public:

  protected:

  private:

  signals:

} ;

class Q_PHOTOGRAPHY_EXPORT PictureView : public SnapView
                                       , public PictureManager
                                       , public Group
{
  Q_OBJECT
  public:

    enum PictureMsgIDs                 {
      PixMsgFocusIn      = 300000001   ,
      PixMsgColors       = 300000002   ,
      PixMsgLocate       = 300000003   ,
      PixMsgJoinS        = 300000004   ,
      PixMsgJoinF        = 300000005   ,
      PixMsgProgress     = 300000006   ,
      PixMsgImport       = 300000008   ,
      PixMsgAppended     = 300000009   ,
      PixMsgExporting    = 300000010   ,
      PixMsgExported     = 300000011   ,
      PixMsgSaveTo       = 300000012   ,
      PixMsgFind         = 300000013   ,
      PixMsgKeyword      = 300000014   ,
      PixMsgSites        = 300000015   ,
      PixMsgOrganize     = 300000016   ,
      PixMsgOrders       = 300000017   ,
      PixMsgToDB         = 300000018   ,
      PixMsgSelected     = 300000019   ,
      PixMsgMoving       = 300000020   ,
      PixMsgImportImages = 300000021   ,
      PixMsgImportFiles  = 300000022   ,
      PixMsgAssignText   = 300000023   ,
      PixMsgAssignHtml   = 300000024   ,
    } ;

    explicit            PictureView     (StandardConstructor) ;
    virtual            ~PictureView     (void) ;

    virtual int         Total           (void) ;
    virtual int         Total           (SUID uuid) ;
    virtual int         TotalOwners     (SUID uuid,int t2) ;
    virtual UUIDs       Pictures        (int StartId,int Size) ;
    virtual UUIDs       Pictures        (SUID uuid,int StartId,int Size) ;
    virtual UUIDs       PictureOwners   (SUID uuid,int t2,int StartId,int Size) ;
    virtual void        Join            (SUID uuid,SUID Before,UUIDs & Uuids) ;

    virtual bool        allowMoving     (void) ;

  protected:

    bool dropAction ;

    virtual bool        FocusIn         (void);

    virtual void        closeEvent      (QCloseEvent * event);

    virtual void        Configure       (void);

    virtual QMimeData * dragMime        (void);

    virtual bool        MovingItems     (QWidget * source,const QMimeData * mime,QPoint pos) ;
    virtual bool        ImportImages    (QWidget * source,const QMimeData * mime,QPoint pos) ;
    virtual bool        ImportFiles     (QWidget * source,const QMimeData * mime,QPoint pos) ;
    virtual bool        PictureObjects  (QWidget * source,const QMimeData * mime,QPoint pos) ;

    virtual bool        acceptDrop      (QWidget * source,const QMimeData * mime) ;
    virtual bool        dropNew         (QWidget * source,const QMimeData * mime,QPoint pos) ;
    virtual bool        dropMoving      (QWidget * source,const QMimeData * mime,QPoint pos) ;
    virtual bool        dropAppend      (QWidget * source,const QMimeData * mime,QPoint pos) ;

    virtual QString     MimeType        (const QMimeData * mime);
    virtual UUIDs       MimeUuids       (const QMimeData * mime,QString mimetype) ;

    virtual bool        dropText        (nDeclDrops,const QString     & text  ) ;
    virtual bool        dropUrls        (nDeclDrops,const QList<QUrl> & urls  ) ;
    virtual bool        dropImage       (nDeclDrops,const QImage      & image ) ;
    virtual bool        dropHtml        (nDeclDrops,const QString     & html  ) ;
    virtual bool        dropTags        (nDeclDrops,const UUIDs       & Uuids ) ;
    virtual bool        dropPictures    (nDeclDrops,const UUIDs       & Uuids ) ;

    virtual void        StopIcons       (void) ;
    virtual void        run             (int Type,ThreadData * data) ;

    virtual bool        Delete          (SqlConnection & Connection,SUID uuid,int type) ;

    virtual bool        isOwner         (void) ;

    virtual bool        PictureId       (SqlConnection & Connection,QListWidgetItem * item) ;
    virtual bool        PictureURLs     (SqlConnection & Connection,QListWidgetItem * item) ;
    virtual bool        PictureDetails  (SqlConnection & Connection,QListWidgetItem * item) ;
    virtual bool        ReportDetails   (SqlConnection & Connection,QListWidgetItem * item,int flags) ;
    virtual void        FetchUUIDs      (void) ;
    virtual void        FetchTOTALs     (void) ;

    virtual void        ExportDirectory (ThreadData * data) ;
    virtual void        ExportUuids     (ThreadData * data) ;
    virtual void        FindFromSites   (ThreadData * data) ;
    virtual void        LocatingOrders  (ThreadData * data) ;
    virtual void        DeleteByThread  (ThreadData * data) ;
    virtual void        AssignText      (ThreadData * data) ;
    virtual void        AssignHtml      (ThreadData * data) ;
    virtual void        ImportFiles     (ThreadData * data) ;
    virtual void        AssignTags      (ThreadData * data) ;

  private:

  public slots:

    virtual bool        startup         (void) ;
    virtual bool        Relocation      (void) ;

    virtual void        View            (UUIDs & Uuids) ;
    virtual void        MoveToEnd       (UUIDs & Uuids) ;
    virtual void        MoveToEnd       (SUID Gallery,UUIDs & Uuids) ;
    virtual void        MoveToEnd       (SUID objectUuid,int objectType,int objectRelation,UUIDs & Uuids) ;

    virtual bool        Reload          (bool maximize = false) ;
    virtual void        Delete          (void) ;
    virtual void        DeleteThis      (void) ;
    virtual void        Home            (void) ;
    virtual void        Previous        (void) ;
    virtual void        Next            (void) ;
    virtual void        End             (void) ;
    virtual void        MoveIndexToEnd  (void) ;
    virtual void        MoveAllToEnd    (void) ;

    virtual void        JoinImage       (const QMimeData * mime) ;
    virtual void        MovePicture     (SUID before,const UUIDs & Uuids) ;

    virtual bool        Import          (QStringList files) ;
    virtual void        Import          (void) ;

    virtual bool        Export          (QDir & dir,UUIDs & Uuids) ;
    virtual void        Save            (void) ;
    virtual void        SaveAll         (void) ;

    virtual void        ReportId        (QListWidgetItem * it) ;
    virtual void        LocateOrders    (void) ;
    virtual void        FindBySites     (void) ;
    virtual void        FindByAlgebra   (void) ;
    virtual void        ReportURLs      (void) ;
    virtual void        Relocate        (SUID objectUuid,int objectType,int objectRelation) ;

    virtual void        addPictureMenu  (MenuManager     & menu       ,
                                         QListWidgetItem * item       ,
                                         SUID              uuid       ,
                                         int             & StartId    ,
                                         int             & PageSize ) ;
    virtual void        addEditingMenu  (MenuManager     & menu       ,
                                         QMenu           * me         ,
                                         QListWidgetItem * item       ,
                                         UUIDs           & Uuids    ) ;
    virtual void        addCatalogMenu  (MenuManager     & menu       ,
                                         QListWidgetItem * item       ,
                                         UUIDs           & Uuids    ) ;
    virtual void        addDetailsMenu  (MenuManager     & menu       ,
                                         QMenu           * mi         ,
                                         QListWidgetItem * item       ,
                                         UUIDs           & Uuids    ) ;
    virtual void        addProcessMenu  (MenuManager     & menu       ,
                                         QListWidgetItem * item       ,
                                         UUIDs           & Uuids    ) ;

    virtual bool        RunCommand      (VarArgs & arguments) ;

  protected slots:

    virtual bool        Menu            (QPoint pos) ;

    void                MoveInternal    (SUID before,UUIDs Uuids) ;
    void                CopyPictures    (SUID before,UUIDs Uuids) ;
    void                ImportPictures  (SUID before,QList<QUrl> urls) ;
    virtual void        DetachIcon      (void) ;
    virtual void        GalleryIcon     (void) ;
    virtual void        GalleryIcon     (SUID FIRST,int TYPE,SUID ICON) ;
    virtual void        GalleryIcon     (VarArgs & arguments) ;

    virtual bool        RunPictureMenu  (MenuManager     & Menu    ,
                                         QAction         * action  ,
                                         QListWidgetItem * item  ) ;
    virtual bool        RunEditingMenu  (MenuManager     & Menu    ,
                                         QAction         * action  ,
                                         QListWidgetItem * item    ,
                                         UUIDs           & Uuids ) ;
    virtual bool        RunCatalogMenu  (MenuManager     & Menu    ,
                                         QAction         * action  ,
                                         QListWidgetItem * item    ,
                                         UUIDs           & Uuids ) ;
    virtual bool        RunDetailsMenu  (MenuManager     & Menu    ,
                                         QAction         * action  ,
                                         QListWidgetItem * item    ,
                                         UUIDs           & Uuids ) ;
    virtual bool        RunProcessMenu  (MenuManager     & Menu    ,
                                         QAction         * action  ,
                                         QListWidgetItem * item    ,
                                         UUIDs           & Uuids ) ;

  private slots:

  signals:

    void                dropInternal    (SUID before,UUIDs Uuids) ;
    void                importPictures  (SUID before,UUIDs Uuids) ;
    void                importPictures  (SUID before,QList<QUrl> urls) ;
    void                Update          (QWidget * widget,SUID uuid) ;
    void                Documents       (QString name,SUID uuid,int type) ;
    void                Constraints     (QString name,SUID uuid,int type) ;
    void                RuleTables      (QString name,SUID uuid,int type) ;
    void                Webpages        (QString name,SUID uuid) ;
    void                RelatedPeople   (QString name,SUID uuid,int type) ;
    void                AllRelated      (QString name,SUID uuid,int type) ;
    void                ListSubgroups   (QString name,SUID uuid,int type) ;
    void                Filters         (QString name,SUID uuid,UUIDs & Uuids) ;
    void                Facial          (QString name,SUID uuid,UUIDs & Uuids) ;
    void                Stitch          (QString name,SUID uuid,UUIDs & Uuids) ;
    void                RunSets         (VarArgLists & Operations) ;
    void                DownloadUUIDs   (UUIDs & Uuids) ;

} ;

class Q_PHOTOGRAPHY_EXPORT ObjectGallery : public PictureView
{
  Q_OBJECT
  public:

    explicit ObjectGallery      (StandardConstructor) ;
    virtual ~ObjectGallery      (void);

  protected:

    virtual bool dropImage      (nDeclDrops,const QImage & image ) ;
    virtual bool dropPictures   (nDeclDrops,const UUIDs  & Uuids ) ;

  private:

  public slots:

    virtual bool Import         (QStringList files) ;
    virtual void MovePicture    (SUID before,const UUIDs & Uuids) ;

    virtual void ImportFaces    (void) ;
    virtual void RecognizeFaces (void) ;

    virtual bool RunCommand     (VarArgs & arguments) ;

  protected slots:

    virtual bool Menu           (QPoint pos) ;

    void joinPictures           (SUID before,UUIDs Uuids) ;
    void joinPictures           (SUID before,QList<QUrl> urls) ;

    virtual void JoinImage      (const QMimeData * mime) ;

  private slots:

  signals:

} ;

class Q_PHOTOGRAPHY_EXPORT DropPictures : public ComboBox
{
  Q_OBJECT
  public:

    explicit     DropPictures    (StandardConstructor) ;
    virtual ~    DropPictures    (void               ) ;

  protected:

    virtual void Configure       (void) ;

    virtual bool acceptDrop      (nDeclWidget,const QMimeData * mime);
    virtual bool dropNew         (nDeclWidget,const QMimeData * mime,QPoint pos);
    virtual bool dropAppend      (nDeclWidget,const QMimeData * mime,QPoint pos);
    virtual bool dropPictures    (nDeclWidget,QPointF pos,const UUIDs & Uuids) ;

  private:

  public slots:

  protected slots:

  private slots:

  signals:

    void         Effect          (SUID ID,UUIDs & Uuids) ;

} ;

class Q_PHOTOGRAPHY_EXPORT PictureDepot : public TreeWidget
{
  Q_OBJECT
  public:

    Sql Depot         ;
    int EachDepot     ;
    int TotalPictures ;

    explicit PictureDepot  (StandardConstructor) ;
    virtual ~PictureDepot  (void) ;

    virtual QSize sizeHint (void) const ;

  protected:

    IMAPs       ProgressId ;
    QStringList Pictures   ;
    QStringList Thumbs     ;

    virtual void Configure (void) ;
    virtual bool FocusIn   (void) ;

    virtual void run       (int type,ThreadData * data) ;

    QStringList Unions     (QString created) ;

  private:

  public slots:

    virtual bool startup   (void) ;
    virtual void Gather    (void) ;
    virtual void Optimize  (void) ;

  protected slots:

    virtual bool Menu      (QPoint pos) ;

  private slots:

  signals:

} ;

class Q_PHOTOGRAPHY_EXPORT GalleryOperator : public Dialog
{
  Q_OBJECT
  public:

    explicit GalleryOperator (StandardConstructor) ;
    virtual ~GalleryOperator (void) ;

    SUID TargetUuid          (void) ;
    SUID SourceUuid          (void) ;

  protected:

    Ui::GalleryOperator * ui ;

  private:

  public slots:

    bool Prepare             (void) ;
    bool Categorize          (void) ;

  protected slots:

  private slots:

  signals:

} ;

class Q_PHOTOGRAPHY_EXPORT GalleryByHost : public Dialog
{
  Q_OBJECT
  public:

    explicit GalleryByHost (StandardConstructor) ;
    virtual ~GalleryByHost (void);

    SUID TargetUuid        (void) ;

  protected:

    Ui::GalleryByHost * ui ;

  private:

  public slots:

    bool Prepare           (void) ;
    bool Categorize        (void) ;

  protected slots:

  private slots:

  signals:

} ;

class Q_PHOTOGRAPHY_EXPORT FeatureExtractor : public QObject
                                            , public Thread
{
  Q_OBJECT
  public:

    explicit FeatureExtractor     (QObject * parent,Plan * plan) ;
    virtual ~FeatureExtractor     (void) ;

  protected:

    Plan               *  plan            ;
    bool                  Commanding      ;
    QTimer             *  CommandSequence ;
    int                   ProgressID      ;
    qint64                ProgressValue   ;
    bool                  ProgressGo      ;
    VarArgLists           Sequences       ;
    QMap<SUID,QImage   *> Images          ;
    QMap<SUID,QByteArray> SurfPoints      ;
    QMap<SUID,int       > SurfTotals      ;
    QMap<SUID,double    > SurfDistances   ;
    QMap<SUID,double    > SurfAngles      ;
    QMap<SUID,double    > SurfX           ;
    QMap<SUID,double    > SurfY           ;
    QMap<SUID,double    > SurfNX          ;
    QMap<SUID,double    > SurfNY          ;
    QMap<SUID,double    > SurfND          ;
    QMap<SUID,QRectF    > SurfRanges      ;
    QMutex                SurfLock        ;

    virtual void Configure        (void) ;

    virtual void run              (int Type,ThreadData * data) ;

  private:

    void         FillIn           (SqlConnection & Connection     ,
                                   QByteArray    & Paraments    ) ;
    void         RunScan          (SqlConnection & Connection     ,
                                   QByteArray    & Paraments      ,
                                   int             TotalThreads   ,
                                   SUID            uuid           ,
                                   int             id             ,
                                   bool          & actual         ,
                                   double          Hessian        ,
                                   int             Octaves        ,
                                   int             Layers         ,
                                   int             Grid           ,
                                   bool            firstTime    ) ;
    bool         CleanTHs         (SqlConnection & Connection     ,
                                   QByteArray    & Paraments      ,
                                   int             TotalThreads ) ;

  public slots:

    virtual bool startup          (void) ;

    void         DigFeaturePoints (void) ;
    void         CalculateFeature (VarArgs arguments) ;

    virtual bool RunCommand       (VarArgs & arguments) ;

  protected slots:

    virtual void StartCommand     (void) ;
    virtual void CommandHandler   (void) ;

  private slots:

    void RelayComplete            (void) ;

  signals:

    void Completed                (void) ;
    void SafeComplete             (void) ;
    void TriggerCommand           (void) ;

} ;

class Q_PHOTOGRAPHY_EXPORT PictureMatcher : public QObject
                                          , public Thread
                                          , public PictureManager
                                          , public GroupItems
{
  Q_OBJECT
  public:

    explicit PictureMatcher     (QObject * parent,Plan * plan) ;
    virtual ~PictureMatcher     (void) ;

  protected:

    QList<void *> Queues    ;
    QMutex        SendMutex ;

    UUIDs        toUuids        (VarArgs & V) ;
    virtual void run            (int Type,ThreadData * data) ;
    virtual void ThreadEvent    (void) ;
    void         showMessage    (QString message) ;

  private:

  public slots:

    virtual void Search         (SUID ID,UUIDs & Uuids) ;

    virtual void SizeMatch      (UUIDs & Uuids) ;
    virtual void ColorsMatch    (UUIDs & Uuids) ;
    virtual void PaletteMatch   (UUIDs & Uuids) ;
    virtual void ExactMatch     (UUIDs & Uuids) ;

  protected slots:

    virtual void AtBusy         (void) ;
    virtual void OnRelax        (void) ;

  private slots:

    void         SendPictures   (void) ;
    void         typeMessage    (QString message) ;

  signals:

    void         OnBusy         (void) ;
    void         GoRelax        (void) ;
    void         PostPictures   (void) ;
    void         sendMessage    (QString message) ;
    void         ViewPictures   (QString name,UUIDs & Uuids) ;

} ;

class Q_PHOTOGRAPHY_EXPORT PicturesDispatcher : public QObject
                                              , public SyntaxDispatcher
                                              , public UuidSyntax
{
  Q_OBJECT
  public:

    explicit     PicturesDispatcher (QObject * parent) ;
    virtual     ~PicturesDispatcher (void) ;

    virtual int  type               (void) const ; // CiosMenuId ( 77 , 0 , 1 )
    virtual int  matching           (QString command) ;
    virtual bool execute            (int id) ;
    virtual void setExtras          (void * data) ;
    virtual void Help               (QStringList & help) ;

  protected:

    Plan              * plan          ;
    WMAPs               Variables     ;
    QList<UUIDs>        PictureQueues ;
    QMap<QString,UUIDs> ListQueues    ;

    virtual bool FetchByTag         (SqlConnection & SC,UUIDs & U,QString Key) ;

  private:

  public slots:

    virtual void FindPictures       (void) ;
    virtual void setVariable        (void) ;
    virtual void showVariables      (void) ;
    virtual void BuildHash          (void) ;
    virtual void PictureUUIDs       (void) ;
    virtual void SearchTags         (void) ;

  protected slots:

  private slots:

    void         SendPicture        (void) ;

  signals:

    void         ViewPicture        (void) ;
    void         Pictures           (QString name,UUIDs & Uuids) ;

} ;

class Q_PHOTOGRAPHY_EXPORT PictureManipulator : public PictureObject
{
  Q_OBJECT
  public:

    explicit     PictureManipulator (QObject * parent,Plan * plan) ;
    explicit     PictureManipulator (QObject * parent,Plan * plan,SceneDestructor * destructor) ;
    virtual     ~PictureManipulator (void) ;

    virtual bool FocusIn            (void) ;
    virtual bool FocusOut           (void) ;

    virtual bool MouseClicked       (ControlPoint & From      ,
                                     ControlPoint & To        ,
                                     BMAPs        & buttons ) ;
    virtual bool MouseVector        (ControlPoint & From      ,
                                     ControlPoint & To        ,
                                     BMAPs        & buttons ) ;
    virtual bool MouseHover         (ControlPoint & From      ,
                                     ControlPoint & To        ,
                                     BMAPs        & buttons ) ;
    virtual bool MouseMove          (ControlPoint & From1     ,
                                     ControlPoint & To1       ,
                                     ControlPoint & From2     ,
                                     ControlPoint & To2       ,
                                     BMAPs        & buttons ) ;
    virtual bool Spot               (ControlPoint & From ,
                                     ControlPoint & To   ,
                                     ControlPoint & At ) ;
    virtual int  AtRegion           (ControlPoint & At) ;

    virtual bool Menu               (QWidget      * widget ,
                                     QPointF        pos    ,
                                     ControlPoint & From   ,
                                     ControlPoint & To   ) ;

    virtual QString ToolTip         (void) ;

    virtual void renderFaces        (void) ;

  protected:

    virtual void Configure          (void) ;

    virtual void run                (int type,ThreadData * data) ;

    virtual QImage * FetchPicture   (SqlConnection & SC,SUID uuid) ;

    virtual void BoxChanged         (void) ;
    virtual void FinalizeSelection  (void) ;

  private:

  public slots:

    virtual bool LoadTexture        (SUID uuid) ;
    virtual void FetchTexture       (SUID uuid) ;

    virtual void ZoomIn             (void) ;
    virtual void ZoomOut            (void) ;
    virtual void NormalRatio        (void) ;
    virtual void PlaceCentral       (void) ;

  protected slots:

    virtual void AdjustMenu         (MenuManager & mm,QMenu   * ma) ;
    virtual bool AdjustProcessor    (MenuManager & mm,QAction * aa) ;

  private slots:

  signals:

} ;

class Q_PHOTOGRAPHY_EXPORT GalleryShelf : public ViewShelf
{
  Q_OBJECT
  public:

    explicit     GalleryShelf       (QObject * parent,Plan * plan) ;
    explicit     GalleryShelf       (QObject * parent,Plan * plan,SceneDestructor * destructor) ;
    virtual     ~GalleryShelf       (void) ;

    virtual bool Menu               (QWidget      * widget ,
                                     QPointF        pos    ,
                                     ControlPoint & From   ,
                                     ControlPoint & To   ) ;

    virtual bool Relocation         (QRectF viewport) ;

  protected:

    virtual void Configure          (void) ;

    virtual bool FocusIn            (void) ;
    virtual bool ItemRunner         (int type,ThreadData * data) ;

    virtual bool hasItem            (ControlPoint & From ,
                                     ControlPoint & To   ,
                                     ControlPoint & At ) ;
    virtual QMimeData * dragMime    (ControlPoint & From ,
                                     ControlPoint & To   ,
                                     ControlPoint & At ) ;
    virtual bool dropPictures       (QWidget     * widget  ,
                                     ControlPoint  pos     ,
                                     const UUIDs & Uuids ) ;
    virtual bool dropDivisions      (QWidget     * widget  ,
                                     ControlPoint  pos     ,
                                     const UUIDs & Uuids ) ;

    virtual void GalleryConstraints (VarArgs & args) ;
    virtual void GalleryRules       (VarArgs & args) ;

    virtual void AllPictures        (void) ;

  private:

  public slots:

    virtual void atDoubleClicked    (SelectItem * item) ;

    virtual void SelectNothing      (void) ;
    virtual void SelectAll          (void) ;
    virtual void NewGallery         (void) ;

    virtual int  Indexing           (RangeBar * rbu,int direction,QVariants & v) ;

  protected slots:

  private slots:

  signals:

    void         ShowAll            (void) ;

} ;

class Q_PHOTOGRAPHY_EXPORT PictureShelf : public ViewShelf
{
  Q_OBJECT
  public:

    explicit     PictureShelf      (QObject * parent,Plan * plan) ;
    explicit     PictureShelf      (QObject * parent,Plan * plan,SceneDestructor * destructor) ;
    virtual     ~PictureShelf      (void) ;

    virtual bool Menu              (QWidget      * widget ,
                                    QPointF        pos    ,
                                    ControlPoint & From   ,
                                    ControlPoint & To   ) ;

    virtual bool Relocation        (QRectF viewport) ;

  protected:

    virtual void Configure         (void) ;

    virtual bool FocusIn           (void) ;
    virtual bool ItemRunner        (int type,ThreadData * data) ;

    virtual bool dropPictures      (QWidget     * widget  ,
                                    ControlPoint  pos     ,
                                    const UUIDs & Uuids ) ;

    virtual void PictureSize       (VarArgs & args) ;
    virtual void PictureOrderId    (VarArgs & args) ;
    virtual void AllPictureSizes   (VarArgs & args) ;

  private:

  public slots:

    virtual void AllPictures       (void) ;
    virtual void ImportPictures    (void) ;

    virtual void SelectNothing     (void) ;
    virtual void SelectAll         (void) ;
    virtual bool ReportDetails     (void) ;

    virtual int  Indexing          (RangeBar * rbu,int direction,QVariants & v) ;

  protected slots:

    virtual void atDoubleClicked   (SelectItem * item) ;

    virtual void PictureMenu       (MenuManager & menu,QMenu   * me,SelectItem * item) ;
    virtual void DetailsMenu       (MenuManager & menu,QMenu   * md,SelectItem * item) ;
    virtual void CatalogMenu       (MenuManager & menu,QMenu   * mc    ) ;
    virtual void FiltersMenu       (MenuManager & menu,QMenu   * mf    ) ;
    virtual bool PictureProcessor  (MenuManager & menu,QAction * action,SelectItem * item) ;
    virtual bool DetailsProcessor  (MenuManager & menu,QAction * action,SelectItem * item) ;
    virtual bool CatalogProcessor  (MenuManager & menu,QAction * action) ;
    virtual bool FiltersProcessor  (MenuManager & menu,QAction * action) ;

  private slots:

  signals:

    void         ShowAll           (void) ;

} ;

class Q_PHOTOGRAPHY_EXPORT SubgroupShelf : public ViewShelf
{
  Q_OBJECT
  public:

    explicit     SubgroupShelf (QObject * parent,Plan * plan) ;
    explicit     SubgroupShelf (QObject * parent,Plan * plan,SceneDestructor * destructor) ;
    virtual     ~SubgroupShelf (void) ;

    virtual bool Menu          (QWidget      * widget ,
                                QPointF        pos    ,
                                ControlPoint & From   ,
                                ControlPoint & To   ) ;

    virtual bool Relocation    (QRectF viewport) ;

  protected:

    virtual void Configure     (void) ;

  private:

  public slots:

    virtual int  Indexing      (RangeBar * rbu,int direction,QVariants & v) ;

  protected slots:

  private slots:

  signals:

} ;

}

QT_END_NAMESPACE

#endif
