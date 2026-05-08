#include <stdio.h>
#include <sys/inotify.h>
#include <sys/wait.h>
#include <unistd.h>

char buffer[4096];
struct inotify_event *event;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <watch-dir>\n", argv[0]);
    return -1;
  }

  int fd = inotify_init();
  if (fd < 0) {
    perror("inotify_init");
    return -1;
  }

  int flags = IN_CREATE | IN_DELETE | IN_MODIFY;

  int wd = inotify_add_watch(fd, argv[1], flags);

  printf("Watching directory %s\n", argv[1]);
  while (1) {
    ssize_t size = read(fd, buffer, sizeof(buffer));
    if (size <= 0) {
      fprintf(stderr, "Error watching.\n");
      return -2;
    }

    for (int i = 0; i < size; i += sizeof(struct inotify_event) + event->len) {
      event = (struct inotify_event *)(buffer + i);

      char *type = (event->mask & IN_ISDIR) ? "Directory" : "File";

      if (event->wd == wd) {
        if (event->mask & IN_CREATE) {
          printf("%s created: %s\n", type, event->name);
        } else if (event->mask & IN_DELETE) {
          printf("%s deleted: %s\n", type, event->name);
        } else if (event->mask & IN_MODIFY) {
          printf("%s modified: %s\n", type, event->name);
        }
      }
    }
  }

  inotify_rm_watch(fd, wd);
  close(fd);
  return 0;
}
