#include "algorithms.h"
#include <iostream>

void BubbleSort(SortVisualizerTool& svt)
{
	for (int i = 0; i < svt.size - 1; i++)
		for (int j = 0; j < svt.size - i - 1; j++)
			if (svt.actCompare(j + 1, j))
				svt.actSwap(j, j + 1);
}
void SelectionSort(SortVisualizerTool& svt)
{
	for (int i = 0; i < svt.size - 1; i++)
	{
		int min = i;
		for (int j = i + 1; j < svt.size; j++)
			if (svt.actCompare(j, min))
				min = j;
		if (min != i)
			svt.actSwap(i, min);
	}
}
void ShellSort(SortVisualizerTool& svt)
{
	for (int i = svt.size/2; i>0; i/=2)
	{
		for (int j = i; j < svt.size; j++)
		{
			for (int k = j; k >= i && svt.actCompare(k, k - i); k -= i)
				svt.actSwap(k, k - i);
		}
	}
}
void InsertionSort(SortVisualizerTool& svt)
{
	for (int i = 1; i < svt.size; i++)
	{
		for (int j = i; j > 0 && svt.actCompare(j, j - 1); j--)
			svt.actSwap(j - 1, j);
	}
}
void merge(SortVisualizerTool& svt, int l, int r)
{
	if (r - l > 1)
	{
		int m = (l + r) / 2, n = r - l;
		merge(svt, l, m);
		merge(svt, m, r);
		int* a = new int[n];
		int li = l, ri = m, ai = 0;
		while (li < m && ri < r && ai < n)
			if (svt.actCompare(ri, li))
				a[ai++] = svt.actGet(ri++);
			else
				a[ai++] = svt.actGet(li++);
		while (li < m && ai < n)
			a[ai++] = svt.actGet(li++);
		while (ri < r && ai < n)
			a[ai++] = svt.actGet(ri++);
		for (int i = 0; i < n; i++)
			svt.actSet(i + l, a[i]);
		delete[]a;
	}
}
void MergeSort(SortVisualizerTool& svt)
{
	merge(svt, 0, svt.size);
}
void quick(SortVisualizerTool& svt, int l, int r)
{
	if (r - l > 1)
	{
		int p = l + 1;
		for (int i = l + 1; i < r; i++)
		{
			if (svt.actCompare(i, l))
			{
				svt.actSwap(p++, i);
			}
		}
		svt.actSwap(p-1, l);
		quick(svt, l, p-1);
		quick(svt, p, r);
	}
}
void QuickSort(SortVisualizerTool& svt)
{
	quick(svt, 0, svt.size);
}
void heap(SortVisualizerTool& svt, int root, int end)
{
	int mx = root;
	int l = root * 2 + 1;
	int r = l + 1;
	if (l < end && svt.actCompare(mx, l))
		mx = l;
	if (r < end && svt.actCompare(mx, r))
		mx = r;
	if (mx != root)
	{
		svt.actSwap(mx, root);
		heap(svt, mx, end);
	}
}
void HeapSort(SortVisualizerTool& svt)
{
	for (int i = svt.size / 2 - 1; i >= 0; i--)
		heap(svt, i, svt.size);
	for (int i = svt.size - 1; i > 0; i--)
	{
		svt.actSwap(0, i);
		heap(svt, 0, i);
	}
}
void RadixSort(SortVisualizerTool& svt, const int base)
{
	int max = svt.size - 1, len = 1;
	while (max)
	{
		max /= base;
		len *= base;
	}
	int* bck = new int[base];
	int* arr = new int[svt.size];
	int* rdx = new int[svt.size];
	for (int i = 1; i < len; i *= base)
	{
		for (int j = 0; j < base; j++)
			bck[j] = 0;
		for (int j = 0; j < svt.size; j++)
		{
			arr[j] = svt.actGet(j);
			rdx[j] = arr[j] / i % base;
			bck[rdx[j]]++;
		}
		for (int j = 0; j < base - 1; j++)
			bck[j + 1] += bck[j];
		for (int j = svt.size - 1; j >= 0; j--)
			svt.actSet(--bck[rdx[j]], arr[j]);
	}
	delete[] bck;
	delete[] arr;
	delete[] rdx;
}