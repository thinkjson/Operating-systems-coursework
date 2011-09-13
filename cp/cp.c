/******************************************************************
 * Copy a file or directory to another location on the filesystem *
 ******************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1
#define FALSE 0
#define TRUE !FALSE

/**
 * Copy a file using UNIX system calls
 */
void copyfile(char *infile, char *outfile)
{
  // Prep the read buffer, infile and outfile
  char buffer[BUFFER_SIZE];
  int bytes_read;
  struct stat inbuffer;

  if (isdir((char*) infile))
  {
    fprintf(stderr, "Cannot copy directory. Try using -r.\n");
    exit(1);
  }

  int in = open(infile, O_RDONLY);
  lstat((char*) infile, &inbuffer);
  int out = open(outfile, O_WRONLY | O_CREAT, inbuffer.st_mode);

  if (in == -1 || out == -1)
  {
    char *source = in == -1 ? "read source" : "create destination";
    char *source_file = in == -1 ? infile : outfile;
    fprintf(stderr, "Could not %s file: %s\n", source, source_file);
    exit(1);
  }

  while((bytes_read = read(in, buffer, BUFFER_SIZE)) > 0)
  {
    write(out, buffer, BUFFER_SIZE);
  }

  if (bytes_read == -1)
  {
    fprintf(stderr, "Could not write to outfile: %s\n", outfile);
    exit(1);
  }
  else
  {
    fprintf(stdout, "%s\n", infile);
  }
}

/**
 * Check to see if a file descriptor refers to a file or a directory
 */
int isdir(const char *filename) {
  struct stat buffer;

  if (lstat(filename, &buffer) == -1) {
    return 0;
  }

  if(S_ISDIR(buffer.st_mode)) {
    return 1;
  }

  return 0;
}

int isvalid(const struct direct *entry)
{
  if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
  {
    return FALSE;
  }
  else
  {
    return TRUE;
  }
}

/**
 * Copy a directory by iterating over contents and calling copyfile()
 */
void copydir(char *indir, char *outdir)
{
  struct direct **files;
  int count, i;
  char *filename;
  char infile[255] = "";
  char outfile[255] = "";
  struct stat inbuffer;

  // Scan directory and iterate over files
  count = scandir(indir, &files, isvalid, NULL);

  if (count == 0)
  {
    return;
  }

  lstat(indir, &inbuffer);
  mkdir(outdir, inbuffer.st_mode);
  for (i = 0; i < count; ++i)
  {
    filename = files[i]->d_name;
    if (isdir(filename))
    {
      fprintf(stdout, "Recursively copying %s\n", filename);
      strcat(infile, indir);
      strcat(infile, "/");
      strcat(infile, filename);
      strcat(outfile, outdir);
      strcat(outfile, "/");
      strcat(outfile, filename);
      copydir(infile, outfile);
    }
    else
    {
      strcat(infile, indir);
      strcat(infile, "/");
      strcat(infile, filename);
      strcat(outfile, outdir);
      strcat(outfile, "/");
      strcat(outfile, filename);
      copyfile(infile, outfile);
    }
  }
}

/**
 * Main function
 */
int main(int argc, char *argv[])
{
  switch (argc)
  {
  case 1:
  case 2:  // Invalid number of arguments
    fprintf(stdout, "Usage is %s [options] source destination\n", argv[0]);
    exit(1);
    break;

  case 3:  // Copying a single file
    // Call copy function
    copyfile(argv[1], argv[2]);
    break;

  case 4:  // Copying a directory recursively
    copydir(argv[2], argv[3]);
    break;
  }

  return 0;
}
