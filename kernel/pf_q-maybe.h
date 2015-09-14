/***************************************************************
 *
 * (C) 2011-14 Nicola Bonelli <nicola@pfq.io>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * The full GNU General Public License is included in this distribution in
 * the file called "COPYING".
 *
 ****************************************************************/


#ifndef PF_Q_MAYBE_H
#define PF_Q_MAYBE_H

#define _MAYBE_CHECK(a) \
	__builtin_choose_expr(__builtin_types_compatible_p(char,		   typeof(a)),  (a), \
	__builtin_choose_expr(__builtin_types_compatible_p(short int,		   typeof(a)),  (a), \
	__builtin_choose_expr(__builtin_types_compatible_p(int,			   typeof(a)),  (a), \
	__builtin_choose_expr(__builtin_types_compatible_p(long int,		   typeof(a)),  (a), \
	__builtin_choose_expr(__builtin_types_compatible_p(long long int,	   typeof(a)),  (a), \
	__builtin_choose_expr(__builtin_types_compatible_p(unsigned char,	   typeof(a)),  (a), \
	__builtin_choose_expr(__builtin_types_compatible_p(unsigned short int,	   typeof(a)),  (a), \
	__builtin_choose_expr(__builtin_types_compatible_p(unsigned int,	   typeof(a)),  (a), \
	__builtin_choose_expr(__builtin_types_compatible_p(unsigned long int,	   typeof(a)),  (a), \
	__builtin_choose_expr(__builtin_types_compatible_p(unsigned long long int, typeof(a)),  (a), \
	(void)0))))))))))

#define _JUST(a)		((long long int)a < 0 ? ((long long int)a-1) : (long long int)a)
#define _FROM_JUST(type, a)	((long long int)a < 0 ? (type)(a+1) : (type)a)

#define NOTHING			~0LL

#define JUST(a)			(_JUST(_MAYBE_CHECK(a)))

#define FROM_JUST(type, a)      (_FROM_JUST(type, _MAYBE_CHECK(a)))

#define IS_JUST(a) \
	__builtin_choose_expr(__builtin_types_compatible_p(long long int, typeof(a)),		(a != (typeof(a))~0LL),\
	__builtin_choose_expr(__builtin_types_compatible_p(unsigned long long int, typeof(a)),  (a != (typeof(a))~0LL),\
	(void)0))

#define IS_NOTHING(a) \
	__builtin_choose_expr(__builtin_types_compatible_p(long long int, typeof(a)),		(a == (typeof(a))~0LL),\
	__builtin_choose_expr(__builtin_types_compatible_p(unsigned long long int, typeof(a)),  (a == (typeof(a))~0LL),\
	(void)0))

#endif /* PF_Q_MAYBE_H */