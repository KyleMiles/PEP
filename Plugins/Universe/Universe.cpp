//Create universe
Universe* universeData = new Universe(0, 0, &bin); //No real loc, but needs bin

//Insert it into the bin so that it can update each tick
bin.insert(universeData);

//Tell every hex in my PEP environment where the universe is
for(Bin::Hex& hex : bin.getAllHexes())
  hex.setUniverseData(universeData);
