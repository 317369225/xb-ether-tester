/* 
 * �����Ϊ��ѡ���Դ�����
 * ������İ�Ȩ(����Դ�뼰�����Ʒ����汾)��һ�й������С�
 * ����������ʹ�á������������
 * ��Ҳ�������κ���ʽ���κ�Ŀ��ʹ�ñ����(����Դ�뼰�����Ʒ����汾)���������κΰ�Ȩ���ơ�
 * =====================
 * ����: ������
 * ����: sunmingbao@126.com
 */

#ifndef  __MISC_UTILS_H__
#define  __MISC_UTILS_H__

#include <pthread.h>
#include <stdint.h>

void nano_sleep(long sec, long nsec);
int set_thread_cpu_affinity(pthread_t thread, int cpu_begin, int cpu_end);

uint64_t rdtsc();
uint64_t get_cpu_freq();
#endif

