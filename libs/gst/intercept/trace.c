#include "trace.h"
#include <stdio.h>

GMutex trace_mutex;

typedef struct TraceEntry
{
  GstPipeline *pipeline;
  const gchar *text;
} TraceEntry;

GList *trace_entries = NULL;

void
gst_pipeline_dump_to_file (GstPipeline *pipeline, const gchar *filename)
{
  GList *iterator;
  g_mutex_lock (&trace_mutex);
  
  FILE *output = fopen (filename, "wt");
  
  for (iterator = g_list_last (trace_entries); iterator != NULL; iterator = iterator->prev)
  {
    TraceEntry *entry = (TraceEntry *)iterator->data;
    if ((pipeline == NULL) || (entry->pipeline == pipeline))
    {
      /*
      if (iterator->prev)
      {
        GList *prev = iterator->prev->prev;
        if (prev)
          prev->next = iterator;
        iterator->prev = prev;
      }
      else
      {
        iterator->prev = NULL;
      }
      */
      
      fprintf(output, "%s\n", entry->text);
    }
  }
  
  fclose (output);
  
  g_mutex_unlock (&trace_mutex);
}

void trace_init (void)
{
  g_mutex_init (&trace_mutex);
}

void
trace_add_entry (GstPipeline *pipeline, const gchar *text)
{
  TraceEntry *entry = g_new0(TraceEntry, 1);
  entry->pipeline = pipeline;
  entry->text = text;
  
  g_mutex_lock (&trace_mutex);
  trace_entries = g_list_prepend (trace_entries, entry);
  g_mutex_unlock (&trace_mutex);
}