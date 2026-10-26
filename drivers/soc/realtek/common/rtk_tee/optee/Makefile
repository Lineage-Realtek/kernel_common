# SPDX-License-Identifier: GPL-2.0
obj-$(CONFIG_REALTEK_OPTEE) += optee.o
ifeq ($(CONFIG_OPTEE_ACL_API),y)
optee-objs += optee_acl_util.o
endif
optee-objs += core.o
optee-objs += call.o
optee-objs += rpc.o
optee-objs += supp.o
optee-objs += shm_pool.o
optee-objs += device.o

# for tracing framework to find optee_trace.h
CFLAGS_call.o := -I$(src)
