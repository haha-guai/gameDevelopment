#pragma once
#include <vector>
#include <graphics.h>

class Atlas
{
public:
	Atlas() = default;
	~Atlas() = default;

	void loadimageFromFiles(LPCTSTR path_template, int num)
	{
		img_list.clear();
		img_list.resize(num);
		TCHAR path_file[256];
		for (size_t i = 0; i < num; i++)
		{
			_stprintf_s(path_file, path_template, i);
			loadimage(&img_list[i], path_file);
		}
	}

	void clear()
	{
		img_list.clear();
	}

	int getSize()
	{
		return (int)img_list.size();
	}

	IMAGE* getImage(int idx)
	{
		if (idx >= img_list.size() || idx < 0)
			return nullptr;
		return &img_list[idx];
	}

	void addImage(IMAGE img)
	{
		img_list.push_back(img);
	}
private:
	std::vector<IMAGE> img_list;
};