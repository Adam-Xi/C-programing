#include<iostream>
using namespace std;

class Date
{
public:

	bool IsInputLegitimate(int year, int month, int day)  //�ж�����ĺϷ���
	{
		if (year > 0)
		{
			if (month > 0 && month < 13)
			{
				if (day > 0 && day <= DaysOfCurMonth(year, month))
				{
					return true;
				}
			}
		}
		return false;
	}
	bool IsLeapYear(int year)  //�ж��Ƿ�����
	{
		if ((0 == year % 4 && 0 != year % 100) || 0 == year % 400)
		{
			return true;
		}
		return false;
	}
	int DaysOfCurMonth(int year, int month)  //�жϵ�ǰ�굱ǰ�µ�����
	{
		int days[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		//    �·�     0   1   2   3   4   5   6   7   8   9  10  11  12
		if (IsLeapYear(year))
		{
			days[month]++;
		}
		return days[month];
	}

	Date(int year = 1999, int month = 1, int day = 1)  //���캯��
	{
		_year = year;
		_month = month;
		_day = day;
	}
	Date(const Date& d)  //�������캯��
	{
		_year = d._year;
		_month = d._month;
		_day = d._day;
	}
	Date& operator=(const Date& d)  //��ֵ���������"="
	{
		if (this != &d)  //��ֹ�Լ����Լ���ֵ
		{
			_year = d._year;
			_month = d._month;
			_day = d._day;
		}
		return *this;
	}
	Date operator+(int days)  //����ǰ���ڼ���days�������
	{
		Date after_days(*this);
		int cur_days = 0;

		days -= (DaysOfCurMonth(_year, _month) - _day);
		while (days > (cur_days = DaysOfCurMonth(_year, ++_month)))
		{
			days %= cur_days;
	}
		if (_month > 12)
		{
			_year += _month / 12;
			_month %= 12;
		}

		return after_days;
	}
	Date operator-(int days)
	{
		Date before_days(*this);
		int cur_days = 0;

		days -= _day;
		while (days > (cur_days = DaysOfCurMonth(_year, --_month)))
		{
			days %= cur_days;
			if (0 == (_month - 1))
			{
				_year--;
				_month = 13;
			}
		}
		_day = days;

		return before_days;
	}
	int operator-(const Date& d)
	{
		int total_days = 0;
		if (operator<(d))
		{
			total_days += (DaysOfCurMonth(_year, _month) - _day) + d._day;

			while (_year < d._year || _month < d._month)
			{
				if ((_month + 1) > 12)
				{
					_year++;
					_month = 0;
				}
				total_days += DaysOfCurMonth(_year, ++_month);
			}
		}
		else if (operator>(d))
		{
			total_days += (DaysOfCurMonth(d._year, d._month) - d._day) + _day;

			while (_year > d._year || _month > d._month)
			{
				if ((_month - 1) == 0)
				{
					_year--;
					_month = 13;
				}
				total_days += DaysOfCurMonth(_year, --_month);
			}
		}
		else
		{
			return 0;
		}
		return total_days;
	}
	Date& operator++()  //ǰ��++
	{
		_day++;
		if (_day > DaysOfCurMonth(_year, _month))
		{
			_month++;
			_day = 1;
			if (_month > 12)
			{
				_year++;
				_month = 1;
			}
		}
		return *this;
	}
	Date operator++(int)  //����++
	{
		Date before_num(*this);
		_day++;
		if (_day > DaysOfCurMonth(_year, _month))
		{
			_month++;
			_day = 1;
			if (_month > 12)
			{
				_year++;
				_month = 1;
			}
		}
		return before_num;
	}
	Date& operator--()  //ǰ��--
	{
		_day--;
		if (_day == 0)
		{
			_month--;
			_day = DaysOfCurMonth(_year, _month);
			if (_month == 0)
			{
				_year--;
				_month = 12;
			}
		}
		return *this;
	}
	Date operator--(int)  //����--
	{
		Date before_num(*this);
		_day--;
		if (_day == 0)
		{
			_month--;
			_day = DaysOfCurMonth(_year, _month);
			if (_month == 0)
			{
				_year--;
				_month = 12;
			}
		}
		return before_num;
	}
	bool operator>(const Date& d)const
	{
		if (_year > d._year || 
			(_year == d._year && _month > d._month) || 
			(_year == d._year && _month == d._month && _day > d._day))
		{
			return true;
		}
		return false;
	}
	bool operator>=(const Date& d)const
	{
		if (_year > d._year || 
			(_year == d._year && _month > d._month) || 
			(_year == d._year && _month == d._month && _day > d._day) || 
			(_year == d._year && _month == d._month && _day == d._day))
		{
			return true;
		}
		return false;
	}
	bool operator<(const Date& d)const
	{
		if (_year < d._year ||
			(_year == d._year && _month < d._month) ||
			(_year == d._year && _month == d._month && _day < d._day))
		{
			return true;
		}
		return false;
	}
	bool operator<=(const Date& d)const
	{
		if (_year < d._year ||
			(_year == d._year && _month < d._month) ||
			(_year == d._year && _month == d._month && _day < d._day) ||
			(_year == d._year && _month == d._month && _day == d._day))
		{
			return true;
		}
		return false;
	}
	bool operator==(const Date& d)const
	{
		if (_year == d._year && _month == d._month && _day == d._day)
		{
			return true;
		}
		return false;
	}
	bool operator!=(const Date& d)const
	{
		if (!operator==(d))
		{
			return true;
		}
		return false;
	}


private:
	int _year;
	int _month;
	int _day;
};

int main()
{
	return 0;
}