/*
 * defines for the malloc-debug library
 *
 * Copyright 1992 by Gray Watson and the Antaire Corporation
 *
 * This file is part of the malloc-debug package.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library (see COPYING-LIB); if not, write to the
 * Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * The author of the program may be contacted at gray.watson@antaire.com
 *
 * $Id: malloc.h,v 1.20 1993/04/06 04:24:34 gray Exp $
 */

#ifndef __MALLOC_H__
#define __MALLOC_H__

/*
 * malloc function return codes
 */
#define CALLOC_ERROR		0		/* error from calloc */
#define MALLOC_ERROR		0		/* error from malloc */
#define REALLOC_ERROR		0		/* error from realloc */

#define FREE_ERROR		0		/* error from free */
#define FREE_NOERROR		1		/* no error from free */

#define MALLOC_VERIFY_ERROR	0		/* checks failed, error */
#define MALLOC_VERIFY_NOERROR	1		/* checks passed, no error */

/*
 * default values if _malloc_file and _malloc_line are not set
 */
#define MALLOC_DEFAULT_FILE	"unknown"
#define MALLOC_DEFAULT_LINE	0

/*
 * global variable and procedure scoping for code readability
 */
#undef	EXPORT
#define	EXPORT

#undef	IMPORT
#define	IMPORT		extern

#undef	LOCAL
#define	LOCAL		static

#if __GNUC__ < 2
/*
 * prototype for memory copy.  needed for below macros.
 */
IMPORT	char	*memcpy(char * to, char * from, int length);
#endif

/*
 * memory copy: copy SIZE bytes from pointer FROM to pointer TO
 */
#define MEMORY_COPY(from, to, size)	(void)memcpy((char *)to, \
						     (char *)from, size)

/*
 * alloc macros to improve memory usage readibility...
 */
#undef ALLOC
#undef CALLOC
#undef MALLOC
#undef REALLOC
#undef REMALLOC
#undef FREE

#define ALLOC(type, count) \
  (type *)malloc((unsigned int)(sizeof(type) * (count)))

#define MALLOC(size) \
  (char *)malloc((unsigned int)(size))

/* WARNING: notice that the arguments are REVERSED from normal calloc() */
#define CALLOC(type, count) \
  (type *)calloc((unsigned int)(count), (unsigned int)sizeof(type))

#define REALLOC(ptr, type, count) \
  (type *)realloc((char *)(ptr), (unsigned int)(sizeof(type) * (count)))

#define REMALLOC(ptr, size) \
  (char *)realloc((char *)(ptr), (unsigned int)(size))

#define FREE(ptr) \
  free((char *)(ptr))

/*
 * some small allocation macros
 */

#ifdef __GNUC__

/*
 * duplicate BUF of SIZE bytes
 */
#define BDUP(buf, size)	({ \
			  char	*_ret; \
			  int	_size = (size); \
			   \
			  _ret = MALLOC(_size); \
			  if (_ret != NULL) \
			    MEMORY_COPY((buf), _ret, _size); \
			   \
			  _ret; \
			})

/*
 * the strdup() function in macro form.  duplicate string STR
 */
#define STRDUP(str)	({ \
			  const char *_strp = (str); \
			  char	*_ret; \
			  int	_len; \
			   \
			  _len = strlen(_strp); \
			  _ret = MALLOC(_len + 1); \
			  if (_ret != NULL) \
			    MEMORY_COPY(_strp, _ret, _len + 1); \
			   \
			  _ret; \
			})

#else /* ! __GNUC__ */

/*
 * duplicate BUF of SIZE and return the new address in OUT
 */
#define BDUP(buf, size, out)	do { \
				  char	*_ret; \
				  int	_size = (size); \
				   \
				  _ret = MALLOC(_size); \
				  if (_ret != NULL) \
				    MEMORY_COPY((buf), _ret, _size); \
				   \
				  (out) = _ret; \
				} while(0)

/*
 * strdup() in macro form.  duplicate string STR and return a copy in OUT
 */
#define STRDUP(str, out)	do { \
				  const char *_strp = (str); \
				  char	*_ret; \
				  int	_len; \
				   \
				  _len = strlen(_strp); \
				  _ret = MALLOC(_len + 1); \
				  if (_ret != NULL) \
				    MEMORY_COPY(_strp, _ret, _len + 1); \
				   \
				  (out) = _ret; \
				} while(0)

#endif /* ! __GNUC__ */

/*<<<<<<<<<<  The below prototypes are auto-generated by fillproto */

/* logfile for dumping malloc info, MALLOC_LOGFILE env. var overrides this */
IMPORT	char		*malloc_logpath;

/* internal malloc error number for reference purposes only */
IMPORT	int		malloc_errno;

/*
 * shutdown memory-allocation module, provide statistics if necessary
 */
IMPORT	void	malloc_shutdown(void);

/*
 * allocate and return a SIZE block of bytes
 */
IMPORT	char	*malloc(unsigned int size);

/*
 * allocate and return a block of bytes able to hold NUM_ELEMENTS of elements
 * of SIZE bytes and zero the block
 */
IMPORT	char	*calloc(unsigned int num_elements, unsigned int size);

/*
 * resizes OLD_PNT to SIZE bytes and return the new space after either copying
 * all of OLD_PNT to the new area or truncating
 */
IMPORT	char	*realloc(char * old_pnt, unsigned int new_size);

/*
 * release PNT in the heap, returning FREE_[NO]ERROR
 */
IMPORT	int	free(char * pnt);

/*
 * call through to _heap_map function, returns [NO]ERROR
 */
IMPORT	int	malloc_heap_map(void);

/*
 * verify pointer PNT, if PNT is 0 then check the entire heap.
 * returns MALLOC_VERIFY_[NO]ERROR
 */
IMPORT	int	malloc_verify(char * pnt);

/*
 * set the global debug functionality flags to DEBUG (0 to disable).
 * returns [NO]ERROR
 */
IMPORT	int	malloc_debug(int debug);

/*
 * examine pointer PNT and returns SIZE, and FILE / LINE info on it
 * if any of the pointers are not NULL.
 * returns NOERROR or ERROR depending on whether PNT is good or not
 */
IMPORT	int	malloc_examine(char * pnt, unsigned int * size,
			       char ** file, unsigned int * line);

/*
 * malloc version of strerror to return the string version of ERRNUM
 * returns the string for MALLOC_BAD_ERRNO if ERRNUM is out-of-range.
 */
IMPORT	char	*malloc_strerror(int errnum);

/*<<<<<<<<<<   This is end of the auto-generated output from fillproto. */

/*
 * alloc macros to provide for memory FILE/LINE debugging information.
 */

#ifndef MALLOC_DEBUG_DISABLE

#define malloc(size) \
  _malloc_leap(__FILE__, __LINE__, size)
#define calloc(count, size) \
  _calloc_leap(__FILE__, __LINE__, count, size)
#define realloc(ptr, size) \
  _realloc_leap(__FILE__, __LINE__, ptr, size)
#define free(ptr) \
  _free_leap(__FILE__, __LINE__, ptr)

#ifdef MALLOC_FUNC_CHECK

/*
 * do debugging on the following functions.  this may cause compilation or
 * other problems depending on your architecture, hence the need for the
 * MALLOC_STRING_DISABLE define.
 */
#define bcmp(b1, b2, len)		_malloc_bcmp(b1, b2, len)
#define bcopy(from, to, len)		_malloc_bcopy(from, to, len

#define memcmp(b1, b2, len)		_malloc_memcmp(b1, b2, len)
#define memcpy(to, from, len)		_malloc_memcpy(to, from, len)
#define memset(buf, ch, len)		_malloc_memset(buf, ch, len)

#define index(str, ch)			_malloc_index(str, ch)
#define rindex(str, ch)			_malloc_rindex(str, ch)

#define strcat(to, from)		_malloc_strcat(to, from)
#define strcmp(s1, s2)			_malloc_strcmp(s1, s2)
#define strlen(str)			_malloc_strlen(str)
#define strtok(str, sep)		_malloc_strtok(str, sep)

#define bzero(buf, len)			_malloc_bzero(buf, len)

#define memccpy(s1, s2, ch, len)	_malloc_memccpy(s1, s2, ch, len)
#define memchr(s1, ch, len)		_malloc_memchr(s1, ch, len)

#define strchr(str, ch)			_malloc_strchr(str, ch)
#define strrchr(str, ch)		_malloc_strrchr(str, ch)

#define strcpy(to, from)		_malloc_strcpy(to, from)
#define strncpy(to, from, len)		_malloc_strncpy(to, from, len)
#define strcasecmp(s1, s2)		_malloc_strcasecmp(s1, s2)
#define strncasecmp(s1, s2, len)	_malloc_strncasecmp(s1, s2, len)
#define strspn(str, list)		_malloc_strspn(str, list)
#define strcspn(str, list)		_malloc_strcspn(str, list)
#define strncat(to, from, len)		_malloc_strncat(to, from, len)
#define strncmp(s1, s2, len)		_malloc_strncmp(s1, s2, len)
#define strpbrk(str, list)		_malloc_strpbrk(str, list)
#define strstr(str, pat)		_malloc_strstr(str, pat)

/*
 * feel free to add your favorite functions here and to malloc_str.[ch]
 */

#endif /* MALLOC_FUNC_CHECK */

/*
 * copied directly from malloc_lp.h
 */

/* to inform the malloc-debug library from which file the call comes from */
IMPORT	char		*_malloc_file;

/* to inform the library from which line-number the call comes from */
IMPORT	unsigned int	_malloc_line;

/*
 * leap routine to calloc
 */
IMPORT	char	*_calloc_leap(const char * file, const int line,
			      unsigned int elen, unsigned int size);

/*
 * leap routine to free
 */
IMPORT	int	_free_leap(const char * file, const int line, char * pnt);

/*
 * leap routine to malloc
 */
IMPORT	char	*_malloc_leap(const char * file, const int line,
			      unsigned int size);

/*
 * leap routine to realloc
 */
IMPORT	char	*_realloc_leap(const char * file, const int line, char * oldp,
			       unsigned int new_size);

#endif /* MALLOC_DEBUG_DISABLE */

#endif /* ! __MALLOC_H__ */
