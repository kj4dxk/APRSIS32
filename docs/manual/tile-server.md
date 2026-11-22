---
title: "Roll Your Own OSM Tile Server"
---

# Roll Your Own OSM Tile Server

[Source wiki page](https://aprsisce.wikidot.com/tile-server) · [Archived HTML](../archive/pages/tile-server--649336064a7f.html) · [Source license](http://creativecommons.org/licenses/by-sa/3.0/)

So, you'd like to run your own OSM Tile Server? It's really not that hard, but if you're looking to serve up the entire planet, it is rather hardware intensive. Here's a collection of random notes of my recently reconstructed (from scratch) OSM tile server.

For the hardware, I use a VMWare Virtual Machine running Ubuntu 16.04.3 LTS Server AMD64. This was not a specific version choice, but the closest version available to match the Switch2OSM instructions to follow (they used 16.04.2 LTS). The physical host of this VM has an 8 core Intel i7, 24GB of RAM, 5 SSDs (1\*120, 2\*240, 2\*500) and multiple SATA-6 magnetic drives. All virtual drives are container files hosted by Windows 7. No drives are fully given over to the VM.

I had a devil of a time getting the VMWare Tools installed in Ubuntu 16.04.3. The kernel header files didn't align with the hgfs device driver that they tried to build. <https://askubuntu.com/questions/580319/enabling-shared-folders-with-open-vm-tools> described using the open-vm-tools and vmhgfs-fuse along with the following commands to mount the VMWare host/guest (hgfs) shared folders.

vmhgfs-fuse .host:/$(vmware-hgfsclient) ~/some\_mountpoint  
sudo mount -t fuse.vmhgfs-fuse .host:/ /mnt/hgfs -o allow\_other

To then make mounting globally persistent add the following line to your /etc/fstab:  
.host:/ /mnt/hgfs fuse.vmhgfs-fuse allow\_other 0 0

The OpenStreetMap folks have published a guide to getting your own OSM tile server up and running. You'll find it at <https://switch2osm.org/manually-building-a-tile-server-16-04-2-lts/> and I followed it (nearly) to the letter. Their instructions are fine for planetary subsets, but the entire planet takes a bit of tweaking (and patience). In particular, their osm2pgsql command does not include —flat-nodes <FlatNodesFile>. The actual command I'm using at the moment is:

osm2pgsql -d gis —create —slim —flat-nodes /mnt/SSDs/flatnodes/flatnodes.dat -G —hstore —tag-transform-script ~/src/openstreetmap-carto/openstreetmap-carto.lua -C 2500 —number-processes 4 -S ~/src/openstreetmap-carto/openstreetmap-carto.style /mnt/SSD/flatnodes/planet-180212.osm.pbf

But I think I have the cache (-C) set too low and my Way imports are suffering. But more on that later.

After following the basic setup instructions and making sure it worked, I set up my SSDs in a striped lvm set using information found at the following URLs. The first is a really great read and the second describes how to utilize ALL of your drives in parallel instead of sequentially.

<https://www.digitalocean.com/community/tutorials/how-to-use-lvm-to-manage-storage-devices-on-ubuntu-16-04>

<https://sysadmincasts.com/episodes/27-lvm-linear-vs-striped-logical-volumes>

And if you decide to add more virtual disks to an existing VM, here's how to get them recognized without rebooting Ubuntu.

<https://www.debuntu.org/how-to-rescan-a-scsi-bus-without-rebooting/>

To get PostGreSQL to use the SSDs by default for data storage in the gis database, I used the "Change tablespace" information from <http://www.postgresqltutorial.com/postgresql-alter-database/> If you don't do this, osm2pgsql will create your new tables in the default postgres data location which will likely be REALLY SLOW.

Speaking of the default postgres data location, apparently some indexing and/or sorting activities still take place in there even after the default tablespace is set on a database. I learned this by watching bwm-ng disk accesses (mentioned in the lvm striping article above) while test-importing both azerbaijan (pre-SSD) and Florida (post-SSD) OSM data extracts. <https://www.digitalocean.com/community/tutorials/how-to-move-a-postgresql-data-directory-to-a-new-location-on-ubuntu-16-04> describes how to move the remainder of your postgresql data to a new location, in my case a three (non-SSD) drive striped lvm.

I had some issues accessing Munin performance graphs from outside the VM and information at ??? provided the proper hint to set up the apache/munin access to allow external viewing.

Some incremental performance stats:

VM @ 21GB RAM (hosted on a 24GB physical machine)  
azerbaijan - 1405 seconds  
florida - 924 seconds

VM @ 12GB RAM (hosted the same, but not causing page faults)  
Also move flatnodes from single SSD to 4 drive striped SSDs  
azerbaijan - 1237 seconds  
florida - 716 seconds

Move postgresql default data to 3 drive striped non-SSD  
florida - 645s

You'll notice the move of flatnodes from the single 120GB SSD to the stripe set. This provides 4\*parallel I/O. Once the planet is imported, I'll be moving flatnodes back to the single drive as it is read-mostly during incremental map data updates.

Handy command (modify as necessary) for watching disk I/O distribution and rates:

bwm-ng -i disk -I xvdb,xvdc,xvdd,xvde,xvdf,xvdg,xvdh,xvdi

Planetary import (-C 2500 - atop says 7.6G cache):  
4331243k Nodes @ 521.1k/sec  
13771k+ Ways @ 0.88k/sec  
??? Relations @ 0.00/sec (note that is NOT k)

Planetary import (-C 10000 (25000 busted) - atop says 383.9M cache):  
974990k Nodes @ 452.1k/sec (flatnodes was already populated (mostly read, not written) even with —create)  
330k Ways @ 0.11k/sec  
??? Relations @ 0.00/sec (note that is NOT k)

Increase VM to 20GB RAM  
Planetary import (-C 15000 (20000 busted) - atop says 383.9M cache):  
974990k Nodes @ 452.1k/sec (flatnodes was already populated (mostly read, not written) even with —create)  
18834k Ways @ 0.46k/sec  
??? Relations @ 0.00/sec (note that is NOT k)

Just learned from <http://www.volkerschatz.com/net/osm/osm2pgsql-usage.html> that:

—number-processes num  
Specifies the number of parallel processes used for certain operations. If disks are fast enough e.g. if you have an SSD, then this can greatly increase speed of the "going over pending ways" and "going over pending relations" stages on a multi-core server. Each of these threads uses the cache size specified with -C, so you may have to reduce it drastically.

So my maximized -C is doomed to failure with my —number-processes 4 as soon as it gets to the "pending ways" section. Oh well, time to restart with a single process for that phase as the cache is demonstrably more important. Going to do a run with -o null to see if that'll give me a count of nodes, ways, and relations so I have a target to watch for on the next import.

A handy source of queries for various database sizes <https://fibrevillage.com/database/66-get-postgresql-db-table-and-index-sizes>
