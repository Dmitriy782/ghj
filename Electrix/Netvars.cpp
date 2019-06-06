#include <random>
#include "Config.h"
#include "Interfaces.h"
#include "Netvars.h"
#include "SDK/CClass.h"
#include "SDK/Entity.h"
#include "Encryptor.h"

static int random(int min, int max) noexcept
{
	static std::mt19937 gen{ std::random_device{ }() };
	return std::uniform_int_distribution{ min, max }(gen);
}

static std::unordered_map<std::string_view, recvProxy> proxies;

constexpr void Hook(RecvProp& prop, recvProxy proxy) noexcept
{
	if (prop.proxy != proxy)
	{
		proxies[prop.name] = prop.proxy;
		prop.proxy = proxy;
	}
}

Netvars::Netvars() noexcept
{
	for (auto CClass = interfaces.client->getAll(); CClass; CClass = CClass->next)
		Load(CClass->recvTable);
}

void Netvars::Load(RecvTable* recvTable, const size_t offset) noexcept
{
    for (int i = 0; i < recvTable->propCount; ++i)
	{
        auto& prop = recvTable->props[i];

        if (isdigit(prop.name[0]))
            continue;
		
        if (prop.dataTable)
			Load(prop.dataTable, prop.offset + offset);
		else
		{
			std::string_view name{ prop.name };
			std::string_view tableName{ recvTable->netTableName };

			offsets[prop.name] = prop.offset + offset;
		}
    }
}