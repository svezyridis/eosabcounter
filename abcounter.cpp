#include <eosiolib/eosio.hpp>
using namespace  eosio;

class abcounter : public contract
{
  public:
    using contract::contract;
    abcounter(account_name self) : contract(self){}

        [[eosio::action]]

      void count(account_name user, std::string type)
    {
        require_auth(N(addressbook));
        count_index counts(_self, _self);
        auto iterator = counts.find(user);
        if (iterator == counts.end())
        {
            counts.emplace(N(addressbook), [&](auto &row) {
                row.key = user;
                row.emplaced = (type == "emplace" ? 1 : 0);
                row.modified = (type == "modify") ? 1 : 0;
                row.erased = (type == "erase") ? 1 : 0;
            });
        }
        else
        {
            counts.modify(iterator, N(addressbook), [&](auto &row) {
                if (type == "emplace")
                {
                    row.emplaced += 1;
                }
                if (type == "modify")
                {
                    row.modified += 1;
                }
                if (type == "erase")
                {
                    row.erased += 1;
                }
            });
        }
    }

  private:
    struct [[eosio::table]] counter
    {
        account_name key;
        uint64_t emplaced;
        uint64_t modified;
        uint64_t erased;
        uint64_t primary_key() const { return key; }
    };
    using count_index = eosio::multi_index<N(counts), counter>;
};

EOSIO_ABI(abcounter, (count));