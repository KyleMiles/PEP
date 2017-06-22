#ifndef SIM_BINS
#define SIM_BINS 1

#include "Entity.h"

//All Active Plugins need to be registered in this file
#include "../Compiler/pluginTypes.h"

#include <map>
#include <set>
#include <functional>
#include <vector>

//Ability to:
    //insert
    //remove
    //get near
    //iterate over each specific type
class Bin// : public byType, public byLocal
{
public:
    //This data structure needs to know the level size in the game
    //Width, height, hexRadius
    Bin(const unsigned int, const unsigned int, const double);

    //How else do we put things in this monstrosity?
    void insert(Entity* const entity);

    //Delete an object
    void remove(Entity* const entity);

    //Update All Entities In The Structure
    void updateEntities();

    ~Bin();
private:
    class Hex;

    //Wrapper for an entity..adds
      //a "unique identifier"
    class Enticap
    {
    public:
        Enticap(Entity* const newEntity, Hex* hexP);

        Hex* getHexP() const;

        ~Enticap(); //be careful with this fellow... Changing how it works could
                    //be really really bad (memory leaks)

        //Entity* UID() const;

        //std::size_t TUID() const;

    private:
        Entity* const entityP;
        Hex* hexP;
    };

    std::vector<unsigned int> hexOffsetCord(const unsigned int x, const unsigned int y) const;

    //For the bins:
    const unsigned int width;
    const unsigned int height;
    const double hexRadius;
    std::vector<std::vector<Hex*>> bins;

    //Setup: <typeid.HashCode: <pointerToEntity:pointerToEnticap> >
    std::map< const std::size_t, std::map<Entity* const, Enticap*> > byTypeMap;

    // vector<Entity* const> allEntities; - have another list of everything?

    //For converting from pixel-hex:
    double guessBinWidth;
    double guessBinHeight;

    std::vector< std::vector< std::vector<Hex*> >> guessGrid;

//Iterstuff - const
public:
    /*
    This was removed as to force the user to use our provided iterators, which
      hide the internals of this data structure
    template<class C>
    const auto& getAllOfTyp() const
    {
        return byTypeMap.at(typeid(C).hash_code());
    }

    This would have been used as such:
    for(auto& flo : bin.getAllOfTyp<Flower>())
        static_cast<Flower*>(flo.first)->thingy = 1;
    for(auto& flo : bin.getAllOfTyp<Flower>())
        cout << static_cast<Flower*>(flo.first)->thingy << " ";
    */

    //If this is deleted it still works... Consider deleting this in the future
    template<class C>
    class byTypeConstIter
    {
        //So the iterator can compare to beginning and end
        friend bool operator!=(
            const byTypeConstIter<C>& lhs,
            const byTypeConstIter<C>& rhs)
        {
            return lhs.iter != rhs.iter;
        }
    public:
        byTypeConstIter(
            const std::map<Entity* const, Enticap*>::const_iterator& iter,
            const std::map< const std::size_t,
            std::map<Entity* const, Enticap*> >* const byTypeMapP)
            :iter(iter), byTypeMapP(byTypeMapP) {}

        const C& operator*() const //I don't like casting, but I know no alt.
        {
            return *static_cast<const C*>(iter->first);
        }

        byTypeConstIter<C>& operator++()
        {
            ++iter;
            return *this;
        }

        byTypeConstIter<C> begin() const
        {
            return byTypeConstIter<C>(
                byTypeMapP->at(typeid(C).hash_code()).begin(), byTypeMapP);
        }

        byTypeConstIter<C> end() const
        {
            return byTypeConstIter<C>(
                byTypeMapP->at(typeid(C).hash_code()).end(), byTypeMapP);
        }

    private:
        std::map<Entity* const, Enticap*>::const_iterator iter;
        const std::map< const std::size_t, std::map<Entity* const, Enticap*> >*
            const byTypeMapP;
    };

    //This returns a const interatable object over a const object
    template<class C>
    byTypeConstIter<C> getAllOfType() const
    {
        return byTypeConstIter<C>(byTypeMap.at( typeid(C).hash_code() ).begin(),
            &byTypeMap);
    }

//Iterstuff - non-const - byType
    template<class C>
    class byTypeIter
    {
        //So the iterator can compare to beginning and end
        friend bool operator!=(
            const byTypeIter<C>& lhs,
            const byTypeIter<C>& rhs)
        {
            return lhs.iter != rhs.iter;
        }
    public:
        byTypeIter(const std::map<Entity* const, Enticap*>::iterator iter,
                   std::map<const std::size_t,
                    std::map<Entity* const, Enticap*> >* const byTypeMapP)
                   :iter(iter), byTypeMapP(byTypeMapP) {}

        C& operator*() //I don't like casting, but I know no alternative
        {
          return *static_cast<C*>((iter)->first);
        }

        byTypeIter<C>& operator++()
        {
            ++iter;
            return *this;
        }

        byTypeIter<C> begin() const
        {
            return byTypeIter<C>(byTypeMapP->at(typeid(C).hash_code()).begin(),
                byTypeMapP);
        }

        byTypeIter<C> end() const
        {
            return byTypeIter<C>(byTypeMapP->at(typeid(C).hash_code()).end(),
                byTypeMapP);
        }

    private:
        std::map<Entity* const, Enticap*>::iterator iter;
        std::map< const std::size_t, std::map<Entity* const, Enticap*> >*
            const byTypeMapP;
    };

    //This returns an interatable object over a non-constant object
    template<class C>
    byTypeIter<C> getAllOfType()
    {
        return byTypeIter<C>(byTypeMap[typeid(C).hash_code()].begin(),
            &byTypeMap);
    }

////////////////////////////////////////////////////////////////////////////////
    //Iterstuff - non-const - byType
    class globalIter
    {
        //So the iterator can compare to beginning and end
        friend bool operator!=(
            const globalIter& lhs,
            const globalIter& rhs)
        {
            return lhs.innerIter != rhs.innerIter;
        }
    public:
        globalIter(const std::map<Entity* const, Enticap*>::iterator innerIter,
                   std::map<const std::size_t, std::map<Entity* const, Enticap*>>::iterator outerIter,
                   std::map<const std::size_t, std::map<Entity* const, Enticap*> >* const byTypeMapP)
                   :innerIter(innerIter), outerIter(outerIter),
                   byTypeMapP(byTypeMapP) {}

        Entity* operator*() //I don't like casting, but I know no alternative
        {
          return innerIter->first;
        }

        globalIter& operator++()
        {
            ++innerIter;

            if(innerIter == outerIter->second.end() &&
               innerIter != byTypeMapP->rbegin()->second.end())
            {
                ++outerIter;
                innerIter = outerIter->second.begin();
            }

            return *this;
        }

        globalIter begin() const
        {
            return globalIter(byTypeMapP->begin()->second.begin(), byTypeMapP->begin(), byTypeMapP);
        }

        globalIter end() const
        {
            return globalIter(byTypeMapP->rbegin()->second.end(), byTypeMapP->end(), byTypeMapP);
        }

    private:
        std::map<Entity* const, Enticap*>::iterator innerIter;
        std::map<const std::size_t, std::map<Entity* const, Enticap*>>::iterator
            outerIter;
        std::map< const std::size_t, std::map<Entity* const, Enticap*> >*
            const byTypeMapP;
    };

    //This returns an interatable object over a non-constant object
    globalIter getAll()
    {
        return globalIter(byTypeMap.begin()->second.begin(), byTypeMap.begin(),
            &byTypeMap);
    }

//hex Iterators:
private:
    #include "Hex.h"

public:
    template<class C>
    byTypeIter<C> getAllOfTypeNear(Entity* entity, int distance = 1)
    {
        distance++;
        return byTypeMap[typeid(C).hash_code()][entity]->getHexP()->getAllOfType<C>();
    }

    globalIter getAllNear(Entity* entity, int distance = 1)
    {
        distance++;
        return byTypeMap[typeid(*entity).hash_code()][entity]->getHexP()->getAll();
    }
};

#endif
