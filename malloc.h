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
 * $Id: malloc.h,v 1.22 1993/04/08 21:46:42 gray Exp $
 */

#ifndef __MALLOC_H__
#define __MALLOC_H__

/*
 * NOTE: some archetectures have malloc, realloc, etc. using size_t or
 * an unsigned long instead of the traditional unsigned int.
 */

#if defined(SIZE_T) || defined(_SIZE_T_) || defined(__SIZE_T__)

#define MALLOC_SIZE	size_t

#else

#define MALLOC_SIZE	unsigned int

#endif

/*
 * malloc function return codes
 */
#define CALLOC_ERROR		0		/* error from calloc */
#define MALLOC_ERROR		0		/* error from malloc */
#define REALLOC_ERROR		0		/* error from realloc */

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
#undef MALLOC_MEM_COPY
#define MALLOC_MEM_COPY(from, to, size)	(void)memcpy((char *)to, \
						     (char *)from, size)

/*
 * alloc macros to improve memory usage readibility...
 */
#undef ALLOC
#define ALLOC(type, count) \
  (type *)malloc((MALLOC_SIZE)(sizeof(type) * (count)))

#undef MALLOC
#define MALLOC(size) \
  (char *)malloc((MALLOC_SIZE)(size))

/* NOTICE: the arguments are REVERSED from normal calloc() */
#undef CALLOC
#define CALLOC(type, count) \
  (type *)calloc((unsigned int)(count), (MALLOC_SIZE)sizeof(type))

#undef REALLOC
#define REALLOC(ptr, type, count) \
  (type *)realloc((char *)(ptr), (MALLOC_SIZE)(sizeof(type) * (count)))

#undef REMALLOC
#define REMALLOC(ptr, size) \
  (char *)realloc((char *)(ptr), (MALLOC_SIZE)(size))

#undef FREE
#define FREE(ptr) \
  free((char *)(ptr))

/*
 * some small allocation macros
 */

#ifdef __GNUC__

/*
 * duplicate BUF of SIZE bytes
 */
#undef BDUP
#define BDUP(buf, size)	({ \
			  char	*_ret; \
			  int	_size = (size); \
			   \
			  _ret = MALLOC(_size); \
			  if (_ret != NULL) \
			    MALLOC_MEM_COPY((buf), _ret, _size); \
			   \
			  _ret; \
			})

/*
 * the strdup() function in macro form.  duplicate string STR
 */
#undef STRDUP
#define STRDUP(str)	({ \
			  const char *_strp = (str); \
			  char	*_ret; \
			  int	_len; \
			   \
			  _len = strlen(_strp); \
			  _ret = MALLOC(_len + 1); \
			  if (_ret != NULL) \
			    MALLOC_MEM_COPY(_strp, _ret, _len + 1); \
			   \
			  _ret; \
			})

#else /* ! __GNUC__ */

/*
 * duplicate BUF of SIZE and return the new address in OUT
 */
#undef BDUP
#define BDUP(buf, size, out)	do { \
				  char	*_ret; \
				  int	_size = (size); \
				   \
				  _ret = MALLOC(_size); \
				  if (_ret != NULL) \
				    MALLOC_MEM_COPY((buf), _ret, _size); \
				   \
				  (out) = _ret; \
				} while(0)

/*
 * strdup() in macro form.  duplicate string STR and return a copy in OUT
 */
#undef STRDUP
#define STRDUP(str, out)	do { \
				  const char *_strp = (str); \
				  char	*_ret; \
				  int	_len; \
				   \
				  _len = strlen(_strp); \
				  _ret = MALLOC(_len + 1); \
				  if (_ret != NULL) \
				    MALLOC_MEM_COPY(_strp, _ret, _len + 1); \
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
#if __STDC__
IMPORT	void	*malloc(MALLOC_SIZE size);
#else
IMPORT	char	*malloc(MALLOC_SIZE size);
#endif

/*
 * allocate and return a block of bytes able to hold NUM_ELEMENTS of elements
 * of SIZE bytes and zero the block
 */
#if __STDC__
IMPORT	void	*calloc(unsigned int num_elements, MALLOC_SIZE size);
#else
IMPORT	char	*calloc(unsigned int num_elements, MALLOC_SIZE size);
#endif

/*
 * resizes OLD_PNT to SIZE bytes and return the new space after either copying
 * all of OLD_PNT to the new area or truncating
 */
#if __STDC__
IMPORT	void	*realloc(void * old_pnt, MALLOC_SIZE new_size);
#else
IMPORT	char	*realloc(char * old_pnt, MALLOC_SIZE new_size);
#endif

/*
 * release PNT in the heap, returning FREE_[NO]ERROR
 */
#if __STDC__
IMPORT	void	free(void * pnt);
#else
IMPORT	int	free(char * pnt);
#endif

/*
 * call through to _heap_map function, returns [NO]ERROR
 */
IMPORT	int	malloc_heap_map(void);

/*
 * verify pointer PNT, if PNT is 0 then check the entire heap.
 * returns MALLOC_VERIFY_[NO]ERROR
 */
#if __STDC__
IMPORT	int	malloc_verify(void * pnt);
#else
IMPORT	int	malloc_verify(char * pnt);
#endif

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
#if __STDC__
IMPORT	int	malloc_examine(void * pnt, MALLOC_SIZE * size,
			       char ** file, unsigned int * line);
#else
IMPORT	int	malloc_examine(char * pnt, MALLOC_SIZE * size,
			       char ** file, unsigned int * line);
#endif

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

#undef malloc
#define malloc(size) \
  _malloc_leap(__FILE__, __LINE__, size)
#undef calloc
#define calloc(count, size) \
  _calloc_leap(__FILE__, __LINE__, count, size)
#undef realloc
#define realloc(ptr, size) \
  _realloc_leap(__FILE__, __LINE__, ptr, size)
#undef free
#define free(ptr) \
  _free_leap(__FILE__, __LINE__, ptr)

#ifdef MALLOC_FUNC_CHECK

/*
 * do debugging on the following functions.  this may cause compilation or
 * other problems depending on your architecture, hence the need for the
 * MALLOC_STRING_DISABLE define.
 */
#undef bcmp
#define bcmp(b1, b2, len)		_malloc_bcmp(b1, b2, len)
#undef bcopy
#define bcopy(from, to, len)		_malloc_bcopy(from, to, len)

#undef memcmp
#define memcmp(b1, b2, len)		_malloc_memcmp(b1, b2, len)
#undef memcpy
#define memcpy(to, from, len)		_malloc_memcpy(to, from, len)
#undef memset
#define memset(buf, ch, len)		_malloc_memset(buf, ch, len)

#undef index
#define index(str, ch)			_malloc_index(str, ch)
#undef rindex
#define rindex(str, ch)			_malloc_rindex(str, ch)

#undef strcat
#define strcat(to, from)		_malloc_strcat(to, from)
#undef strcmp
#define strcmp(s1, s2)			_malloc_strcmp(s1, s2)
#undef strlen
#define strlen(str)			_malloc_strlen(str)
#undef strtok
#define strtok(str, sep)		_malloc_strtok(str, sep)

#undef bzero
#define bzero(buf, len)			_malloc_bzero(buf, len)

#undef memccpy
#define memccpy(s1, s2, ch, len)	_malloc_memccpy(s1, s2, ch, len)
#undef memchr
#define memchr(s1, ch, len)		_malloc_memchr(s1, ch, len)

#undef strchr
#define strchr(str, ch)			_malloc_strchr(str, ch)
#undef strrchr
#define strrchr(str, ch)		_malloc_strrchr(str, ch)

#undef strcpy
#define strcpy(to, from)		_malloc_strcpy(to, from)
#undef strncpy
#define strncpy(to, from, len)		_malloc_strncpy(to, from, len)
#undef strcasecmp
#define strcasecmp(s1, s2)		_malloc_strcasecmp(s1, s2)
#undef strncasecmp
#define strncasecmp(s1, s2, len)	_malloc_strncasecmp(s1, s2, len)
#undef strspn
#define strspn(str, list)		_malloc_strspn(str, list)
#undef strcspn
#define strcspn(str, list)		_malloc_strcspn(str, list)
#undef strncat
#define strncat(to, from, len)		_malloc_strncat(to, from, len)
#undef strncmp
#define strncmp(s1, s2, len)		_malloc_strncmp(s1, s2, len)
#undef strpbrk
#define strpbrk(str, list)		_malloc_strpbrk(str, list)
#undef strstr
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
			      unsigned int elen, MALLOC_SIZE size);

/*
 * leap routine to free
 */
IMPORT	int	_free_leap(const char * file, const int line, char * pnt);

/*
 * leap routine to malloc
 */
IMPORT	char	*_malloc_leap(const char * file, const int line,
			      MALLOC_SIZE size);

/*
 * leap routine to realloc
 */
IMPORT	char	*_realloc_leap(const char * file, const int line, char * oldp,
			       MALLOC_SIZE new_size);

#endif /* ! MALLOC_DEBUG_DISABLE */

#endif /* ! __MALLOC_H__ */
