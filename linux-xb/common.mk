# 
# �����Ϊ��ѡ���Դ�����
# ������İ�Ȩ(����Դ�뼰�����Ʒ����汾)��һ�й������С�
# ����������ʹ�á������������
# ��Ҳ�������κ���ʽ���κ�Ŀ��ʹ�ñ����(����Դ�뼰�����Ʒ����汾)���������κΰ�Ȩ���ơ�
# =====================
# ����: ������
# ����: sunmingbao@126.com
# 

# CROSS_COMPILE=arm-linux-
export CC:=$(CROSS_COMPILE)gcc
export CFLAGS:=-c -O2 -Wall -fno-strict-overflow -fno-strict-aliasing
export LD:=$(CROSS_COMPILE)ld
export AR:=$(CROSS_COMPILE)ar

export RM:=rm -rf
export MKDIR:=mkdir -p

export LDFLAGS:=-rdynamic
export C_LIBS:=-ldl -lpthread 