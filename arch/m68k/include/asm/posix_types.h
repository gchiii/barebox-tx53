/*
 * Copyright (c) 2008 Carsten Schlote <c.schlote@konzeptpark.de>
 * See file CREDITS for list of people who contributed to this project.
 *
 * This file is part of U-Boot V2.
 *
 * U-Boot V2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * U-Boot V2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with U-Boot V2.  If not, see <http://www.gnu.org/licenses/>.
 */

/** @file
 *  This file is generally used by user-level software, so you need to
 *  be a little careful about namespace pollution etc.  Also, we cannot
 *  assume GCC is being used.
 */
#ifndef __ARCH_M68K_POSIX_TYPES_H
#define __ARCH_M68K_POSIX_TYPES_H


typedef unsigned short		__kernel_dev_t;
typedef unsigned long		__kernel_ino_t;
typedef unsigned short		__kernel_mode_t;
typedef unsigned short		__kernel_nlink_t;
typedef long			__kernel_off_t;
typedef int			__kernel_pid_t;
typedef unsigned short		__kernel_ipc_pid_t;
typedef unsigned short		__kernel_uid_t;
typedef unsigned short		__kernel_gid_t;
typedef unsigned int		__kernel_size_t;
typedef int			__kernel_ssize_t;
typedef int			__kernel_ptrdiff_t;
typedef long			__kernel_time_t;
typedef long			__kernel_suseconds_t;
typedef long			__kernel_clock_t;
typedef int			__kernel_daddr_t;
typedef char *			__kernel_caddr_t;
typedef unsigned short		__kernel_uid16_t;
typedef unsigned short		__kernel_gid16_t;
typedef unsigned int		__kernel_uid32_t;
typedef unsigned int		__kernel_gid32_t;

typedef unsigned short		__kernel_old_uid_t;
typedef unsigned short		__kernel_old_gid_t;

#ifdef __GNUC__
typedef long long		__kernel_loff_t;
#endif

typedef struct {
#if defined(__KERNEL__) || defined(__USE_ALL)
	int	val[2];
#else /* !defined(__KERNEL__) && !defined(__USE_ALL) */
	int	__val[2];
#endif /* !defined(__KERNEL__) && !defined(__USE_ALL) */
} __kernel_fsid_t;

#if defined(__KERNEL__) || !defined(__GLIBC__) || (__GLIBC__ < 2)

#undef	__FD_SET
#define __FD_SET(fd, fdsetp) \
		(((fd_set *)fdsetp)->fds_bits[fd >> 5] |= (1<<(fd & 31)))

#undef	__FD_CLR
#define __FD_CLR(fd, fdsetp) \
		(((fd_set *)fdsetp)->fds_bits[fd >> 5] &= ~(1<<(fd & 31)))

#undef	__FD_ISSET
#define __FD_ISSET(fd, fdsetp) \
		((((fd_set *)fdsetp)->fds_bits[fd >> 5] & (1<<(fd & 31))) != 0)

#undef	__FD_ZERO
#define __FD_ZERO(fdsetp) \
		(memset (fdsetp, 0, sizeof (*(fd_set *)fdsetp)))

#endif

#endif