#ifndef LIBNSS_ROLE_MANAGER_H_
#define LIBNSS_ROLE_MANAGER_H_

#include <string>

#include <Role/RoleCommon.h>
#include <Role/LockFile.h>
#include <Role/PamCheck.h>

class RoleManager;
std::ostream& operator <<(std::ostream &os, const RoleManager &manager);

class RoleManager
{
public:
	typedef std::vector<std::string> PrivNames;
private:
	int fd;
	Roles roles;
	std::string config;
	PamCheck pamcheck;
	LockFile locker;
	GroupMap groupmap;

	void fillGroups(Groups &groups, const PrivNames &list, bool skip = false);
	Privs getPrivs(const Groups &groups);
public:
	RoleManager(const std::string &config, const std::string &progname);
	~RoleManager();

	void Update();
	void Store();

	void Add(const std::string &name, const PrivNames &list, bool skip = false);
	void Set(const std::string &name, const PrivNames &list, bool skip = false);
	void Delete(const std::string &name, const PrivNames &list, bool skip = false);
	void Remove(const std::string &name);

	friend std::ostream& operator <<(std::ostream &os, const RoleManager &manager);
};

#endif /*LIBNSS_ROLE_MANAGER_H_*/
