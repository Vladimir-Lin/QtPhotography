#include <qtphotography.h>

#pragma message("AttachGallery need to change into newer implementation")

N::AttachGallery:: AttachGallery (QWidget * parent,Plan * p)
                 : GalleryView   (          parent,       p)
                 , AttachUuid    (0                        )
                 , AttachType    (Types::Project           )
{
  WidgetClass ;
}

N::AttachGallery::~AttachGallery (void)
{
}

UUIDs N::AttachGallery::GroupUuids(SqlConnection & Connection)
{
  return Subordination                         (
           Connection                          ,
           AttachUuid                          ,
           AttachType                          ,
           type                                ,
           Groups::Subordination               ,
           Connection.OrderByAsc("position") ) ;
}

SUID N::AttachGallery::AppendGroup(SqlConnection & Connection,QString name)
{
  UUIDs Uuids                                                                 ;
  SUID  u = GroupItems::NewDivision(Connection,name,(Types::ObjectTypes)type) ;
  Uuids << u                                                                  ;
  GroupItems::Join(Connection,AttachUuid,AttachType,type                      ,
                   Groups::Subordination,0,Uuids        )                     ;
  return u                                                                    ;
}
