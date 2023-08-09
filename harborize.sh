#sed -i s#git-change-log#sfdk-changelog# rpm/info.you_ra.screen_recorder.changes.run

mv rpm/info.you_ra.screen_recorder.changes.run rpm/info.you_ra.screen_recorder.changes.run.in

sed -i s#auroraapp#sailfishapp# rpm/info.you_ra.screen_recorder.spec
sed -i s#auroraapp#sailfishapp# CMakeLists.txt 
sed -i s#AURORA#SAILFISH# CMakeLists.txt
sed -i s#auroraapp#sailfishapp# src/main.cpp
sed -i s#Aurora#SailfishApp# src/main.cpp
sed -i s#::Application::#::# src/main.cpp
# rename in files
for f in 'CMakeLists.txt' 'rpm/info.you_ra.screen_recorder.spec' 'info.you_ra.screen_recorder.desktop' ; do sed -i s#info.you_ra#harbour-info.you_ra# $f; done

# rename files
for f in `find ./ -name "info.you*"`; do mv $f "$(echo "$f" | sed s#info#harbour-info#)"; done



