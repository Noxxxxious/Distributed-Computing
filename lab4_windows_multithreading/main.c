# include <windows.h>
# include <stdio.h>
# include <conio.h>
# include <math.h>
# include <time.h>
#define MAX_THREAD_COUNT 16
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# pragma argsused
struct thread_data
{
	int thread_id;
	float a;
	float b;
	float section;
	float result;
};
DWORD WINAPI thread_function(void* args);
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int main(int argc, char** argv)
{
	//zad1
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	//CreateProcess(TEXT("C:\\Windows\\System32\\notepad.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

	//zad2
	system("cls");
	HANDLE threads[MAX_THREAD_COUNT];
	struct thread_data data[MAX_THREAD_COUNT];
	int priority_arr[MAX_THREAD_COUNT];
	float time_log[MAX_THREAD_COUNT];
	clock_t time;
	float area = 0;
	float a = -2.0, b = 3.0;
	int thread_count;
	for (thread_count = 1; thread_count <= MAX_THREAD_COUNT; thread_count++) {
		area = 0;
		int i;
		float section_w = (b - a) / MAX_THREAD_COUNT;
		for (i = 0; i < thread_count; i++) {
			struct thread_data tmp = { i + 1, a + i * section_w, b - (thread_count - i - 1) * section_w, section_w, 0 };
			data[i] = tmp;
			priority_arr[i] = i - 1;
		}
		DWORD id;
		time = clock();
		for (i = 0; i < thread_count; i++)
		{
			threads[i] = CreateThread(NULL, 0, thread_function, (void*)&data[i], 0, &id);
			if (threads[i] != INVALID_HANDLE_VALUE)
			{
				SetThreadPriority(threads[i], priority_arr[i]);
			}
		}
		time = clock() - time;
		time_log[thread_count - 1] = (float)time / CLOCKS_PER_SEC;
		WaitForMultipleObjects(thread_count, threads, 1, 1000);
		for (i = 0; i < thread_count; i++) {
			area += data[i].result;
		}
	}
	
	Sleep(5000);
	printf("integral from %f to %f: %f\n", a, b, area);
	printf("calculation times:\n");
	int i;
	for (i = 0; i < MAX_THREAD_COUNT; i++) {
		printf("%f, ", time_log[i]);
	}
	return 0;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
DWORD WINAPI thread_function(void* args)
{
	struct thread_data* data = (struct thread_data*)args;
	float a = data->a;
	float b = data->b;
	data->result = ((pow(a, 3.0) - pow(a, 2.0) + a + pow(b, 3.0) - pow(b, 2.0) + b) * data->section) / 2.0;
	Sleep(1000);
	return 0;
}