# 
# �����Ϊ��ѡ���Դ�����
# ������İ�Ȩ(����Դ�뼰�����Ʒ����汾)��һ�й������С�
# ����������ʹ�á������������
# ��Ҳ�������κ���ʽ���κ�Ŀ��ʹ�ñ����(����Դ�뼰�����Ʒ����汾)���������κΰ�Ȩ���ơ�
# =====================
# ����: ������
# ����: sunmingbao@126.com
# 

#e.g. arm-linux-gnueabihf-
CROSS_COMPILE ?=
	
#e.g. /opt/gcc-linaro-5.4.1-2017.05-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc
SYS_ROOT ?= /

export CC:=$(CROSS_COMPILE)gcc
export CFLAGS:=--sysroot=$(SYS_ROOT) -c -O2 -Wall -fno-strict-overflow -fno-strict-aliasing
export LD:=$(CROSS_COMPILE)ld
export AR:=$(CROSS_COMPILE)ar

export RM:=rm -rf
export MKDIR:=mkdir -p

export LDFLAGS:=-rdynamic
export C_LIBS:=-ldl -lpthread 