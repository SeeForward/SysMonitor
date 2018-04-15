#include "disk_info.h"
#include "system_tool.h"

#ifdef __WINDOWS__
#	include <Tlhelp32.h>
#else
#	include <stdio.h>
#	include <unistd.h>
#	include <string.h>
#	include <stdlib.h>
#	include <mntent.h>
#	include <fcntl.h>
#	include <sys/param.h>
#	include <sys/vfs.h>
#	include <sys/ioctl.h>
#	include <linux/fs.h>
#	include <linux/hdreg.h>
#	include <linux/major.h>
#	include <scsi/sg.h>
#	include <scsi/scsi_ioctl.h>
#endif //__WINDOWS__

using std::vector;
using std::string;

bool DiskInfo::GetLogiDisks(vector<LogiDisk> &vLd)
{
	vLd.clear();
#ifdef __WINDOWS__
	char nameStrs[512];
	nameStrs[0] = '\0';
	//Get the names of logical disks
	if (!::GetLogicalDriveStrings(sizeof(nameStrs), nameStrs))
	{
		return false;
	}
	vector<string> vecName;
	char *cp = nameStrs;
	while (*cp)
	{
		vecName.push_back(cp);
		cp += strlen(cp) + 1;
	};
	
	LogiDisk ld;
	uint64_t usedBytes = 0;	
	for (size_t i = 0; i < vecName.size(); ++i) 
	{
		//Ignore floppy disk
		if ('A' == vecName[i][0] || 'B' == vecName[i][0])
		{
			continue;
		}

		if (!::GetDiskFreeSpaceEx(vecName[i].c_str(), 
								(PULARGE_INTEGER)&ld.m_avail, 
								(PULARGE_INTEGER)&ld.m_total, 
								(PULARGE_INTEGER)&usedBytes)) 
		{
			continue;
		}
		ld.m_name = vecName[i];
		if (ld.m_total > 0)
		{
			ld.m_usage = (float)(100 * (ld.m_total - ld.m_avail) / (double) ld.m_total);
		}
		vLd.push_back(ld);
	}
#else
	FILE *fp = setmntent("/proc/mounts", "r");
	if (!fp) 
	{
		return false;
	}

	LogiDisk ld;
	struct mntent *me = NULL;
	struct statfs diskInfo;
	while ((me = getmntent(fp))) 
	{
		if (0 == strncmp(me->mnt_type, "rootfs", 6))
		{
			continue;
		}
		if (strstr(me->mnt_type, "tmpfs"))
		{
			continue;
		}
		ld.m_name = me->mnt_fsname;

		if (0 != statfs(me->mnt_dir,&diskInfo))
		{
			continue;
		}
		//The size of bytes in a block
		uint32_t blockSize = diskInfo.f_bsize;

		ld.m_total = blockSize * diskInfo.f_blocks;
		if (0 == ld.m_total)
		{
			endmntent(fp);
			return false;
		}
		ld.m_avail = diskInfo.f_bfree * blockSize;
		ld.m_usage = 100 * (ld.m_total - ld.m_avail) / (double)ld.m_total;

		vLd.push_back(ld);
	}
	endmntent(fp);
#endif //__WINDOWS__
	return true;
}

bool DiskInfo::GetPhysDisks(vector<PhysDisk> &vPd)
{
	vPd.clear();
#ifdef __WINDOWS__

	PhysDisk pd;
	char buf[1024];
	char drivePath [64];
	char name[4];
	//Max number of physical disk is 16
	for (int drive = 0; drive < 16; ++drive)  
	{
		sprintf(name, "%d", drive);
		pd.m_name = name;
		
		sprintf(drivePath, "\\\\.\\PhysicalDrive%d", drive);  

		HANDLE hIoctl = ::CreateFile(drivePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (INVALID_HANDLE_VALUE == hIoctl)
		{
			break;
		}

		STORAGE_PROPERTY_QUERY query;  
		memset((void *)&query, 0, sizeof(query));  
		query.PropertyId = StorageDeviceProperty;  
		query.QueryType = PropertyStandardQuery;  

		memset(buf, 0, sizeof(buf));  
		DWORD cbBytesReturned = 0;  
		if (::DeviceIoControl(hIoctl, IOCTL_STORAGE_QUERY_PROPERTY, &query, sizeof(query), &buf, sizeof(buf), &cbBytesReturned, NULL))
		{
			STORAGE_DEVICE_DESCRIPTOR * descrip = (STORAGE_DEVICE_DESCRIPTOR *)&buf;
			
			pd.m_model = (char*)(buf + descrip->ProductIdOffset);

			// Get the disk drive geometry
			memset (buf, 0, sizeof(buf));
			if (::DeviceIoControl (hIoctl, IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, NULL, 0, &buf, sizeof(buf), &cbBytesReturned, NULL))
			{
				DISK_GEOMETRY_EX* geom = (DISK_GEOMETRY_EX*)&buf;   
				pd.m_total = geom->DiskSize.QuadPart;
			}
			else
			{
				pd.m_total = 0;
			}
		}
		::CloseHandle (hIoctl);

		vPd.push_back(pd);
	}
	if (vPd.empty())
	{
		return false;
	}
#else
	vector<string> vPdn;
	if (!GetPhysDiskName(vPdn))
	{
		return false;
	}
	vPd.reserve(vPdn.size());

	PhysDisk pd;
	for (size_t i = 0; i < vPdn.size(); ++i)
	{
		//Ignore floppy disk
		if (strstr(vPdn[i].c_str(), "fd"))
		{
			continue;
		}
		pd.m_name = vPdn[i];
		pd.m_model = GetPhysDiskModel(vPdn[i]);
		pd.m_total = GetPhysDiskSize(vPdn[i]);

		vPd.push_back(pd);
	}
#endif //__WINDOWS__
	return true;
}

#ifndef __WINDOWS__

bool DiskInfo::GetPhysDiskName(vector<string>& vPdn)
{
	vPdn.clear();

	FILE *fp = fopen("/proc/diskstats", "r");
	if (!fp)
	{
		return false;
	}

	char name[32];
	char buf[1024];
	int major = 0;
	int minor = 0;
	int reads = 0;
	while (fgets(buf, sizeof(buf), fp)) 
	{
		if (4 == sscanf(buf, "%4d %4d %31s %u", &major, &minor, name, &reads)) 
		{
			if (reads && IsPhysDisk(major, minor))
			{
				vPdn.push_back(name);				
			}
		}
	}
	fclose(fp);

	return true;
}

string DiskInfo::GetPhysDiskModel(const string& name)
{
	string strName = "/dev/" + name;
	int fd = open(strName.c_str(), O_RDONLY | O_NONBLOCK);
	if (fd < 0)
	{
		return "";
	}
	//For IDE disk
	if (name[0] == 'h') 
	{
		struct hd_driveid hid;

		if (0 != ioctl(fd,HDIO_GET_IDENTITY,&hid))
		{
			return "";
		}
		close(fd);
		return string((char*)hid.model);
	}
	//For SCSI disk
	else if (name[0] == 's') 
	{
		struct sg_io_hdr * p_hdr = new sg_io_hdr;
		if (!p_hdr) 
		{
			return "";
		}
		memset(p_hdr, 0, sizeof(struct sg_io_hdr));
		
		p_hdr->interface_id = 'S';
		p_hdr->flags = SG_FLAG_LUN_INHIBIT;

		unsigned char data_buf[32*256];
		p_hdr->dxferp = data_buf;
		p_hdr->dxfer_len = sizeof(data_buf);
		
		unsigned char sense_buf[255];
		p_hdr->sbp = sense_buf;
		p_hdr->mx_sb_len = sizeof(sense_buf);

		unsigned char cdb[6];
		cdb[0] = 0x12;
		cdb[1] = 0 & 1;
		cdb[2] = 0 & 0xff;
		cdb[3] = 0;
		cdb[4] = 0xff;
		cdb[5] = 0; 
		
		p_hdr->dxfer_direction = SG_DXFER_FROM_DEV;
		p_hdr->cmdp = cdb;
		p_hdr->cmd_len = 6;

		if (0 != ioctl(fd, SG_IO, p_hdr))
		{
			close(fd);
			delete p_hdr;
			return "";
		}

		string model((char*)p_hdr->dxferp + 8, (char*)p_hdr->dxferp + 36); //[8 -- 36]

		close(fd);
		delete p_hdr;
		return model;
	}
	return "";
}

uint64_t DiskInfo::GetPhysDiskSize(const string& name)
{
	string strName = "/dev/" + name;
	int fd = open(strName.c_str(), O_RDONLY | O_NONBLOCK);
	if (fd < 0) 
	{
		return 0;
	}

	uint64_t size = 0;
	int order = 0;
	if ('s' == name[0])
	{
		order = BLKGETSIZE64;
	}
	else if ('h' == name[0])
	{
		order = BLKGETSIZE;
	}

	if (0 != ioctl(fd, order, &size))
	{
		close(fd);
		return 0;
	}

	close(fd);
	return size;
}
#endif //__LINUX__
