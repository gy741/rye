/*
 * Copyright (C) 2008 Search Solution Corporation. All rights reserved by Search Solution.
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */


/*
 * intl_support.h : internationalization support interfaces
 */

#ifndef _INTL_SUPPORT_H_
#define _INTL_SUPPORT_H_

#ident "$Id$"

#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <limits.h>

#include <wchar.h>

#include "dbtype.h"

#ifndef MB_LEN_MAX
#define MB_LEN_MAX            1
#endif

#ifndef WEOF
#define WEOF                  (wint_t)(-1)
#endif


/* next UTF-8 char */
#define INTL_NEXTCHAR_UTF8(c) \
  (unsigned char*)((c) + intl_Len_utf8_char[*(unsigned char*)(c)])

/* next UTF-8 char and its length */
#define INTL_GET_NEXTCHAR_UTF8(c,l) { \
    l = intl_Len_utf8_char[*(unsigned char*)(c)]; \
    c += (l); \
  }

/* !!! Do not use this for BIT type !!! see STR_SIZE macro */
#define INTL_CODESET_MULT INTL_UTF8_MAX_CHAR_SIZE

/* Checks if string having charset 'cs_from' can be safely reinterpreted as
 * having charset 'cs_to'.
 * All strings can be reinterpreted as ISO-8859-1 charset.
 * Other combinations are not compatible, since 8 bit values are starter for
 * mutibyte characters.
 */
#define INTL_CAN_STEAL_CS(cs_from,cs_to)  \
    ((cs_from) == (cs_to) || (cs_to) == INTL_CODESET_ISO88591)

/* Checks if string having charset 'cs_from' can be coerced (transformed) as
 * having charset 'cs_to'.
 * All strings can be transformed to ISO-8859-1 charset by reinterpreting data
 * The other transformations require charset conversion.
 * In some cases, the destination charset may not contain an encoding of the
 * original character, and is replaced with '?' character (ASCII 3f)
 */
#define INTL_CAN_COERCE_CS(cs_from,cs_to)  true

#define INTL_NEXT_CHAR(ptr, s, current_char_size) \
  (ptr) = intl_nextchar_utf8 ((s), (current_char_size))

/* language identifier : we support built-in languages and user defined
 * languages (through locale definition);
 * User defined languages are assigned IDs after built-in languages IDs
 * It is not guaranteed that user defined languages keep their IDs */
typedef unsigned int INTL_LANG;

typedef enum intl_builtin_lang INTL_BUILTIN_LANG;
enum intl_builtin_lang
{
  INTL_LANG_ENGLISH = 0,
  INTL_LANG_KOREAN,
  INTL_LANG_USER_DEF_START
};

typedef enum intl_zone INTL_ZONE;
enum intl_zone
{
  INTL_ZONE_US,
  INTL_ZONE_KR
};

typedef enum intl_codeset INTL_CODESET;
enum intl_codeset
{
  INTL_CODESET_ERROR = -2,
  INTL_CODESET_NONE = -1,
  INTL_CODESET_UTF8,		/* UNICODE charset, UTF-8 encoding */

  INTL_CODESET_LAST = INTL_CODESET_UTF8
};

/* map of lengths of UTF-8 characters */
extern const unsigned char *const intl_Len_utf8_char;

#ifdef __cplusplus
extern "C"
{
#endif

#if defined (ENABLE_UNUSED_FUNCTION)
  extern int intl_tolower_iso8859 (unsigned char *s, int length);
  extern int intl_toupper_iso8859 (unsigned char *s, int length);

  extern unsigned char *intl_nextchar_euc (unsigned char *s,
					   int *curr_length);
  extern unsigned char *intl_prevchar_euc (unsigned char *s,
					   const unsigned char *s_start,
					   int *prev_length);
#endif
  extern unsigned char *intl_nextchar_utf8 (unsigned char *s,
					    int *curr_length);
  extern unsigned char *intl_prevchar_utf8 (unsigned char *s,
					    const unsigned char *s_start,
					    int *prev_length);

  extern INTL_ZONE intl_zone (int category);

  extern int intl_char_count (unsigned char *src, int length_in_bytes,
			      int *char_count);
  extern int intl_char_size (unsigned char *src, int length_in_chars,
			     int *byte_count);
  extern unsigned char *intl_prev_char (unsigned char *s,
					const unsigned char *s_start,
					int *prev_char_size);
  extern unsigned char *intl_next_char (unsigned char *s,
					int *current_char_size);
  extern int intl_cmp_char (const unsigned char *s1, const unsigned char *s2,
			    int *char_size);
  extern void intl_pad_char (unsigned char *pad_char, int *pad_size);
  extern int intl_pad_size (void);
  extern int intl_upper_string_size (const void *alphabet,
				     unsigned char *src, int src_size,
				     int src_length);
  extern int intl_upper_string (const void *alphabet,
				unsigned char *src, unsigned char *dst,
				int length_in_chars);
  extern int intl_lower_string_size (const void *alphabet,
				     unsigned char *src, int src_size,
				     int src_length);
  extern int intl_lower_string (const void *alphabet,
				unsigned char *src, unsigned char *dst,
				int length_in_chars);
  extern int intl_reverse_string (unsigned char *src, unsigned char *dst,
				  int length_in_chars, int size_in_bytes);
  extern bool intl_is_max_bound_chr (const unsigned char *chr);
  extern bool intl_is_min_bound_chr (const unsigned char *chr);
  extern int intl_set_min_bound_chr (char *chr);
  extern int intl_set_max_bound_chr (char *chr);
  extern int intl_case_match_tok (const INTL_LANG lang_id,
				  unsigned char *tok, unsigned char *src,
				  const int size_tok, const int size_src,
				  int *matched_size_src);
  extern int intl_identifier_casecmp (const char *str1, const char *str2);
  extern int intl_identifier_cmp (const char *str1, const char *str2);
  extern int intl_identifier_lower (const char *src, char *dst);
  extern int intl_identifier_upper (const char *src, char *dst);
  extern int intl_identifier_fix (char *name, int ident_max_size);
  extern int intl_put_char (unsigned char *dest, const unsigned char *char_p);
  extern bool intl_is_space (const char *str, const char *str_end,
			     int *space_size);
  extern const char *intl_skip_spaces (const char *str, const char *str_end);
  extern const char *intl_backskip_spaces (const char *str_begin,
					   const char *str_end);
  extern char *intl_mbs_chr (const char *mbs, wchar_t the_char);
  extern int intl_mbs_spn (const char *mbs, const wchar_t * chars);
  extern int intl_mbs_len (const char *mbs);

  extern const char *intl_mbs_nth (const char *mbs, size_t n);
  extern char *intl_mbs_ncpy (char *mbs1, const char *mbs2, size_t n);
  extern int intl_mbs_casecmp (const char *mbs1, const char *mbs2);
  extern int intl_mbs_ncasecmp (const char *mbs1, const char *mbs2, size_t n);
  extern int intl_cp_to_utf8 (const unsigned int codepoint,
			      unsigned char *utf8_seq);
#if defined (ENABLE_UNUSED_FUNCTION)
  extern int intl_cp_to_dbcs (const unsigned int codepoint,
			      const unsigned char *byte_flag,
			      unsigned char *seq);
#endif
  extern unsigned int intl_utf8_to_cp (const unsigned char *utf8,
				       const int size,
				       unsigned char **next_char);
#if defined (ENABLE_UNUSED_FUNCTION)
  extern unsigned int intl_back_utf8_to_cp (const unsigned char *utf8_start,
					    const unsigned char *utf8_last,
					    unsigned char
					    **last_byte__prev_char);
#endif
  extern unsigned int intl_dbcs_to_cp (const unsigned char *seq,
				       const int size,
				       const unsigned char *byte_flag,
				       unsigned char **next_char);
  extern int intl_utf8_to_cp_list (const unsigned char *utf8, const int size,
				   unsigned int *cp_array,
				   const int max_array_size,
				   int *array_count);
  extern int intl_text_single_byte_to_utf8 (const char *in_buf,
					    const int in_size, char **out_buf,
					    int *out_size);
  extern int intl_text_single_byte_to_utf8_ext (void *txt_conv,
						const char *in_buf,
						const int in_size,
						char **out_buf,
						int *out_size);
  extern int intl_text_utf8_to_single_byte (const char *in_buf,
					    const int in_size, char **out_buf,
					    int *out_size);
#if defined (ENABLE_UNUSED_FUNCTION)
  extern int intl_text_dbcs_to_utf8 (const char *in_buf, const int in_size,
				     char **out_buf, int *out_size);
  extern int intl_text_dbcs_to_utf8_ext (void *t, const char *in_buf,
					 const int in_size, char **out_buf,
					 int *out_size);
  extern int intl_text_utf8_to_dbcs (const char *in_buf, const int in_size,
				     char **out_buf, int *out_size);
  extern int intl_fast_iso88591_to_utf8 (const unsigned char *in_buf,
					 const int in_size,
					 unsigned char **out_buf,
					 int *out_size);
  extern int intl_euckr_to_utf8 (const unsigned char *in_buf,
				 const int in_size, unsigned char **out_buf,
				 int *out_size);
  extern int intl_utf8_to_euckr (const unsigned char *in_buf,
				 const int in_size, unsigned char **out_buf,
				 int *out_size);
  extern int intl_iso88591_to_euckr (const unsigned char *in_buf,
				     const int in_size,
				     unsigned char **out_buf, int *out_size);
#endif
  extern int intl_count_utf8_chars (unsigned char *s, int length_in_bytes);
  extern bool intl_check_utf8 (const unsigned char *buf,
			       int size, char **pos);
  extern bool intl_check_euckr (const unsigned char *buf, int size,
				char **pos);

  extern bool intl_is_bom_magic (const char *buf, const int size);

#ifdef __cplusplus
}
#endif

#endif				/* _INTL_SUPPORT_H_ */
