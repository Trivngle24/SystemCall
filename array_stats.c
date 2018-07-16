#include "array_stats.h"
#include <linux/kernel.h>
#include <linux/uaccess.h>


asmlinkage long sys_array_stats(struct array_stats *stats, long data[],	long size)
{

	int i;
	long min = 0;
	long max = 0;
	long sum = 0;
	long tmp = 0;
	struct array_stats temp;

	if(size <= 0)
		return -EINVAL;
	
	if(copy_from_user(&tmp, &data[0], sizeof(long)))
		return -EFAULT;

	min = tmp;
	max = tmp;
	sum = tmp;
	
	for(i=1; i<size; i++){		
		if(copy_from_user(&tmp, &data[i], sizeof(long)))
			return -EFAULT;
		sum += tmp;
		if(min > tmp)
			min = tmp;
		if(max < tmp)
			max = tmp;
	}

	temp.min = min;
	temp.max = max;
	temp.sum = sum;

	if(copy_to_user(&(stats->min), &(temp.min), sizeof(long)) 
		|| copy_to_user(&(stats->max), &(temp.max), sizeof(long))
		|| copy_to_user(&(stats->sum), &(temp.sum), sizeof(long))){
		return -EFAULT;
	}	

	
	return 0;


}
