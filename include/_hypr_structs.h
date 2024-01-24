#ifndef __hypr_structs__h
#define __hypr_structs__h

struct workspaceTile {
  int index;
  int monitor_id;
  char icon[8];
  char css_class[32];
};
typedef struct workspaceTile workspaceTile;

#endif //__hypr_structs__h
