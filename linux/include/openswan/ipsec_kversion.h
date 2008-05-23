#ifndef _OPENSWAN_KVERSIONS_H
/*
 * header file for FreeS/WAN library functions
 * Copyright (C) 1998, 1999, 2000  Henry Spencer.
 * Copyright (C) 1999, 2000, 2001  Richard Guy Briggs
 * Copyright (C) 2003 - 2008  Paul Wouters <paul@xelerance.com>
 * 
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Library General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/lgpl.txt>.
 * 
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
 * License for more details.
 *
 */
#define	_OPENSWAN_KVERSIONS_H	/* seen it, no need to see it again */

/*
 * this file contains a series of atomic defines that depend upon
 * kernel version numbers. The kernel versions are arranged
 * in version-order number (which is often not chronological)
 * and each clause enables or disables a feature.
 */

/*
 * First, assorted kernel-version-dependent trickery.
 */
#include <linux/version.h>
#ifndef KERNEL_VERSION
# define KERNEL_VERSION(x,y,z) (((x)<<16)+((y)<<8)+(z))
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,1,0)
# define HEADER_CACHE_BIND_21
# error "KLIPS is no longer supported on Linux 2.0. Sorry"
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,1,0)
# define SPINLOCK
# define PROC_FS_21
# define NETLINK_SOCK
# define NET_21
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,1,19)
# define net_device_stats enet_statistics
#endif                                                                         

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,3,0)
# define SPINLOCK_23
# define NETDEV_23
# ifndef CONFIG_IP_ALIAS
#  define CONFIG_IP_ALIAS
# endif
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,3,25)
# define PROC_FS_2325
# undef  PROC_FS_21
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,3,30)
# define PROC_NO_DUMMY
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,3,35)
# define SKB_COPY_EXPAND
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,3,37)
# define IP_SELECT_IDENT
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,3,50)
# if(LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23) && defined(CONFIG_NETFILTER))
#  define SKB_RESET_NFCT
# elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,23)
#  if defined(CONFIG_NF_CONNTRACK) || defined(CONFIG_NF_CONNTRACK_MODULE)
#   define SKB_RESET_NFCT
#  endif
# endif
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,2)
# define IP_SELECT_IDENT_NEW
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,4)
# define IPH_is_SKB_PULLED
# define SKB_COW_NEW
# define PROTO_HANDLER_SINGLE_PARM
# define IP_FRAGMENT_LINEARIZE 1
#else /* LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,4) */
#  ifdef REDHAT_BOGOSITY
#  define IP_SELECT_IDENT_NEW
#  define IPH_is_SKB_PULLED
#  define SKB_COW_NEW
#  define PROTO_HANDLER_SINGLE_PARM
#  endif /* REDHAT_BOGOSITY */
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,4) */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,9)
# define MALLOC_SLAB
# define LINUX_KERNEL_HAS_SNPRINTF
#endif                                                                         

/* API changes are documented at: http://lwn.net/Articles/2.6-kernel-api/ */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
# define HAVE_NETDEV_PRINTK 1
# define NET_26
# define NETDEV_25
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,8)
# define NEED_INET_PROTOCOL
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,12)
# define HAVE_SOCK_ZAPPED
# define NET_26_12_SKALLOC
#endif

/* see <linux/security.h> */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,13)
# define HAVE_SOCK_SECURITY
/* skb->nf_debug disappared completely in 2.6.13 */
# define HAVE_SKB_NF_DEBUG
#endif

/* skb->stamp changed to skb->tstamp in 2.6.14 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,14)
# define HAVE_TSTAMP
# define HAVE_INET_SK_SPORT
#else
# define HAVE_SKB_LIST 
#endif

#define SYSCTL_IPSEC_DEFAULT_TTL sysctl_ip_default_ttl                      
/* it seems 2.6.14 accidentally removed sysctl_ip_default_ttl */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,14)
# undef  SYSCTL_IPSEC_DEFAULT_TTL
# define SYSCTL_IPSEC_DEFAULT_TTL IPSEC_DEFAULT_TTL
#endif

/*
   The obsolete MODULE_PARM() macro is gone forevermore [in 2.6.17+]
    It was introduced in 2.6.0
   Zero-filled memory can now be allocated from slab caches with
    kmem_cache_zalloc(). There is also a new slab debugging option
    to produce a /proc/slab_allocators file with detailed allocation
    information.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
# define module_param(a,b,c)  MODULE_PARM(#a,"i")
/* note below is only true for our current calls to module_param_array */
# define module_param_array(a,b,c,d)  MODULE_PARM(#a,"1-2i")
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18)
/*
   The skb_linearize() function has been reworked, and no longer has a
    GFP flags argument. There is also a new skb_linearize_cow() function
    which ensures that the resulting SKB is writable.
   Network drivers should no longer manipulate the xmit_lock  spinlock
    in the net_device structure; instead, the following new functions
    should be used:
     int netif_tx_lock(struct net_device *dev);
     int netif_tx_lock_bh(struct net_device *dev);
     void netif_tx_unlock(struct net_device *dev);
     void netif_tx_unlock_bh(struct net_device *dev);
     int netif_tx_trylock(struct net_device *dev);
   A number of crypto API changes have been merged, the biggest being
    a change to most algorithm-specific functions to take a pointer to
    the crypto_tfm structure, rather than the old "context" pointer. This
    change was necessary to support parameterized algorithms.
*/

# define HAVE_NEW_SKB_LINEARIZE
#endif

/* this is the best we can do to detect XEN, which makes
 * patches to linux/skbuff.h, making it look like 2.6.18 version 
 */
#ifdef CONFIG_XEN
# define HAVE_NEW_SKB_LINEARIZE
#endif

/* And the same for SuSe kernels who have it before it got into the
 * linus kernel.
 */
#ifdef SLE_VERSION_CODE
# if SLE_VERSION_CODE >= 655616
#  define HAVE_NEW_SKB_LINEARIZE
# else
#  warning "A Suse kernel was detected, but we are unsure if it requires HAVE_NEW_SKB_LINEARIZE"
# endif
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,19)
# define VOID_SOCK_UNREGISTER
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
/* skb->nfmark changed to skb->mark in 2.6.20 */
# define nfmark mark
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,21)
/*
   Significant changes have been made to the crypto support interface.
   The sysctl code has been heavily reworked, leading to a number of
    internal API changes. 
*/
# define ipsec_register_sysctl_table(a,b) register_sysctl_table(a)
# define CTL_TABLE_PARENT
#else
# define ipsec_register_sysctl_table(a,b) register_sysctl_table(a,b)
#endif
 
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,22)
/*
   The eth_type_trans() function now sets the skb->dev field, consistent
    with how similar functions for other link types operate. As a result,
    many Ethernet drivers have been changed to remove the (now) redundant
    assignment.
   The header fields in the sk_buff structure have been renamed
    and are no longer unions. Networking code and drivers can
    now just use skb->transport_header, skb->network_header, and
    skb->skb_mac_header. There are new functions for finding specific
    headers within packets: tcp_hdr(), udp_hdr(), ipip_hdr(), and
    ipipv6_hdr().
   The crypto API has a new set of functions for use with asynchronous
    block ciphers. There is also a new cryptd kernel thread which can
    run any synchronous cipher in an asynchronous mode.
   A new macro has been added to make the creation of slab caches easier:
    struct kmem_cache KMEM_CACHE(struct-type, flags);
    The result is the creation of a cache holding objects of the given
     struct_type, named after that type, and with the additional slab
     flags (if any). 
*/

/* need to include ip.h early, no longer pick it up in skbuff.h */
# include <linux/ip.h>
# define HAVE_KERNEL_TSTAMP
/* type of sock.sk_stamp changed from timeval to ktime  */
# define grab_socket_timeval(tv, sock)  { (tv) = ktime_to_timeval((sock).sk_stamp); }
#else
# define grab_socket_timeval(tv, sock)  { (tv) = (sock).sk_stamp; }
/* internals of struct skbuff changed */
# define        HAVE_DEV_NEXT
# define ip_hdr(skb)  ((skb)->nh.iph)
# define skb_tail_pointer(skb)  ((skb)->tail)
# define skb_end_pointer(skb)  ((skb)->end)
# define skb_network_header(skb)  ((skb)->nh.raw)
# define skb_set_network_header(skb,off)  ((skb)->nh.raw = (skb)->data + (off))
# define tcp_hdr(skb)  ((skb)->h.th)
# define udp_hdr(skb)  ((skb)->h.uh)
# define skb_transport_header(skb)  ((skb)->h.raw)
# define skb_set_transport_header(skb,off)  ((skb)->h.raw = (skb)->data + (off))
# define skb_mac_header(skb)  ((skb)->mac.raw)
# define skb_set_mac_header(skb,off)  ((skb)->mac.raw = (skb)->data + (off))
#endif
/* turn a pointer into an offset for above macros */
#define ipsec_skb_offset(skb, ptr) (((unsigned char *)(ptr)) - (skb)->data)

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,23)
/* 
 * The macro got introduced in 2,6,22 but it does not work properly, and
 * still uses the old number of arguments. 
 */
 /*
    The destructor argument has been removed from kmem_cache_create(), as
    destructors are no longer supported. All in-kernel callers have been
    updated
  */
# define HAVE_KMEM_CACHE_MACRO

/* Try using the new kernel encaps hook for nat-t, instead of udp.c */
# ifdef NOT_YET_FINISHED
#  define HAVE_UDP_ENCAP_CONVERT
# endif

#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24)
/*
 * We can switch on earlier kernels, but from here on we have no choice
 * but to abandon the old style proc_net and use seq_file
 */
# define HAVE_SEQ_FILE
#else
/*
   The hard_header() method has been removed from struct net_device;
    it has been replaced by a per-protocol header_ops structure pointer. 

   The prototype for slab constructor callbacks has changed to:
    void (*ctor)(struct kmem_cache *cache, void *object);
   The unused flags argument has been removed and the order of the other
    two arguments has been reversed to match other slab functions. 
*/
# define HAVE_DEV_HARD_HEADER
#endif

#ifdef NET_21
# include <linux/in6.h>
#else
     /* old kernel in.h has some IPv6 stuff, but not quite enough */
# define	s6_addr16	s6_addr
# define	AF_INET6	10
# define uint8_t __u8
# define uint16_t __u16 
# define uint32_t __u32 
# define uint64_t __u64 
#endif

#if __KERNEL__
# if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,0)
#  if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,0) 
#   include "openswan/ipsec_kern24.h"
#  else
#   error "kernels before 2.4 are not supported at this time"
#  endif
# endif
#endif

#endif /* _OPENSWAN_KVERSIONS_H */

