#include "queue.h"
#include <stdlib.h>

typedef struct _QueueNodeSt {
  void      *item_;
  struct _QueueNodeSt *next_node_;
}QueueNodeSt;

typedef struct {
  QueueNodeSt     *read_node_;
  QueueNodeSt     *write_node_;
  int             node_num_;
}QueueInfoSt;

QueueFd * QueueCreate(void)
{
  QueueInfoSt *fd = (QueueInfoSt *)malloc(sizeof(QueueInfoSt));
  if (fd == NULL) {
    return NULL;
  }
  fd->node_num_ = 0;
  fd->read_node_ = NULL;
  fd->write_node_ = NULL;
  return fd;
}

void QueueDestroy(QueueFd * fd)
{
  QueueClear(fd);
  free(fd);
}

void QueueClear(QueueFd * fd)
{
  QueueInfoSt *info_st = (QueueInfoSt *)fd;
  void *item = NULL;
  while (info_st->read_node_ != NULL) {
    item = QueueRetrieve(fd);
    if (item != NULL) {
      free(QueueRetrieve(fd));
    }
  }
}

int QueueAppend(QueueFd * fd, void * item)
{
  QueueInfoSt *info_st = (QueueInfoSt *)fd;
  QueueNodeSt *node = (QueueNodeSt *)malloc(sizeof(QueueNodeSt));
  if (node == NULL) {
    return -1;
  }
  node->next_node_ = NULL;
  node->item_ = item;
  if (info_st->write_node_ == NULL) {
    info_st->write_node_ = node;
    info_st->read_node_ = info_st->write_node_;
  }
  else {
    info_st->write_node_->next_node_ = node;
    info_st->write_node_ = info_st->write_node_->next_node_;
  }
  info_st->node_num_++;
  return 0;
}

void * QueueRetrieve(QueueFd * fd)
{
  QueueInfoSt *info_st = (QueueInfoSt *)fd;
  if (info_st->read_node_ == NULL) {
    return NULL;
  }
  QueueNodeSt *node = info_st->read_node_;
  info_st->read_node_ = info_st->read_node_->next_node_;
  if (info_st->read_node_ == NULL) {
    info_st->write_node_ = NULL;
  }
  info_st->node_num_--;
  void *item = node->item_;
  free(node);
  return item;
}

int QueueLength(QueueFd * fd)
{
  QueueInfoSt *info_st = (QueueInfoSt *)fd;
  return info_st->node_num_;
}
