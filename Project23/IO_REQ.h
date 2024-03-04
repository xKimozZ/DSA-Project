#ifndef IO
#define IO
class IO_REQ
{
public:
	int io_time;
	int io_dur;
	int ac_dur;
	void setIO_DUR(int l)
	{
		io_dur = l;
	}
};
#endif

