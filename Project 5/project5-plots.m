%% Google stuff wrong
clear 
clc
A = readmatrix('times-google.txt');

hold on
average = mean(A,'omitnan');
for i = 1:size(A,2)
    %plot(i,A(:,i),'.','Color'[1,0,0,0.2])
    scatter(i,A(:,i),'o','MarkerEdgeAlpha',0.2)
    plot(i,average(i),"-s",'MarkerFaceColor','r',MarkerSize=10)
end
xlabel('Hop')
ylabel('Time (ms)')
title('Scatter plot of times to reach routers for google.com')
%% UTexas stuff wrong
clear
clc
A = readmatrix('times-utexas.txt');

hold on 
average = mean(A,'omitnan');
for i = 1:size(A,2)
    %plot(i,A(:,i),'.','Color'[1,0,0,0.2])
    scatter(i,A(:,i),'o','MarkerEdgeAlpha',0.2)
    plot(i,average(i),"-s",'MarkerFaceColor','r',MarkerSize=10)
end
xlabel('Hop')
ylabel('Time (ms)')
title('Scatter plot of times to reach routers for utexas.edu')
%% icir stuff wrong
clear 
clc
hold on
A = readmatrix('times-icir.txt');
average = mean(A,'omitnan');
for i = 1:size(A,2)
    %plot(i,A(:,i),'.','Color'[1,0,0,0.2])
    scatter(i,A(:,i),'o','MarkerEdgeAlpha',0.2)
    plot(i,average(i),"-s",'MarkerFaceColor','r',MarkerSize=10)
end
xlabel('Hop')
ylabel('Time (ms)')

title('Scatter plot of times to reach routers for icir.org')
%% NYU stuff wrong
clear
clc
hold on

A = readmatrix('times-nyu.txt');
average = mean(A,'omitnan');
for i = 1:size(A,2)
    %plot(i,A(:,i),'.','Color'[1,0,0,0.2])
    scatter(i,A(:,i),'o','MarkerEdgeAlpha',0.2)
    plot(i,average(i),"-s",'MarkerFaceColor','r',MarkerSize=10)
end
xlabel('Hop')
ylabel('Time (ms)')
title('Scatter plot of times to reach routers for nyu.org')
%% wrong
clear
clc
hold on

A = readmatrix('times-utoronto.txt');
average = mean(A,'omitnan');
for i = 1:size(A,2)
    %plot(i,A(:,i),'.','Color'[1,0,0,0.2])
    scatter(i,A(:,i),'o','MarkerEdgeAlpha',0.2)
    plot(i,average(i),"-s",'MarkerFaceColor','r',MarkerSize=10)
end
xlabel('Hop')
ylabel('Time (ms)')
title('Scatter plot of times to reach routers for utoronto')
%% wrong
clear 
clc
hold on

A = readmatrix('times-govza.txt');
average = mean(A,'omitnan');
for i = 1:size(A,2)
    %plot(i,A(:,i),'.','Color'[1,0,0,0.2])
    scatter(i,A(:,i),'o','MarkerEdgeAlpha',0.2)
    plot(i,average(i),"-s",'MarkerFaceColor','r',MarkerSize=10)
end
xlabel('Hop')
ylabel('Time (ms)')
title('Scatter plot of times to reach routers for gov.za')

xlim([0 inf])
ylim([0 inf])
%% 46.188.47.18 wrong
clear 
clc
hold on

A = readmatrix('times-46.188.47.18.txt');
average = mean(A,'omitnan');
for i = 1:size(A,2)
    %plot(i,A(:,i),'.','Color'[1,0,0,0.2])
    scatter(i,A(:,i),'o','MarkerEdgeAlpha',0.2)
    plot(i,average(i),"-s",'MarkerFaceColor','r',MarkerSize=10)
end
xlabel('Hop')
ylabel('Time (ms)')
title('Scatter plot of times to reach routers for Moscow')

xlim([0 inf])
ylim([0 inf])
%% 82.71.8.205
clear 
clc 
hold on

A = readmatrix('times-82.71.8.205.txt');

average = mean(A,'omitnan');
for i = 1:size(A,2)
    %plot(i,A(:,i),'.','Color'[1,0,0,0.2])
    scatter(i,A(:,i),'o','MarkerEdgeAlpha',0.2)
    plot(i,average(i),"-s",'MarkerFaceColor','r',MarkerSize=10)
end
xlabel('Hop')
ylabel('Time (ms)')
title('Scatter plot of times to reach routers for London')

xlim([0 inf])
ylim([0 inf])
%% 212.102.51.18
clear 
clc

hold on
A = readmatrix('times-212.102.51.18.txt');
average = mean(A,'omitnan');
for i = 1:size(A,2)
    %plot(i,A(:,i),'.','Color'[1,0,0,0.2])
    scatter(i,A(:,i),'o','MarkerEdgeAlpha',0.2)
    plot(i,average(i),"-s",'MarkerFaceColor','r',MarkerSize=10)
end
xlabel('Hop')
ylabel('Time (ms)')
title('Scatter plot of times to reach routers for Tokyo')

xlim([0 inf])
ylim([0 inf])
%% 43.229.60.176
clear 
clc

hold on
A = readmatrix('times-43.229.60.176.txt');
average = mean(A,'omitnan');
for i = 1:size(A,2)
    %plot(i,A(:,i),'.','Color'[1,0,0,0.2])
    scatter(i,A(:,i),'o','MarkerEdgeAlpha',0.2)
    plot(i,average(i),"-s",'MarkerFaceColor','r',MarkerSize=10)
end
xlabel('Hop')
ylabel('Time (ms)')
title('Scatter plot of times to reach routers for Sydney')

xlim([0 inf])
ylim([0 inf])
%% 210.5.56.145
clear 
clc

hold on
A = readmatrix('times-210.5.56.145.txt');
average = mean(A,'omitnan');
for i = 1:size(A,2)
    %plot(i,A(:,i),'.','Color'[1,0,0,0.2])
    scatter(i,A(:,i),'o','MarkerEdgeAlpha',0.2)
    plot(i,average(i),"-s",'MarkerFaceColor','r',MarkerSize=10)
end
xlabel('Hop')
ylabel('Time (ms)')
title('Scatter plot of times to reach routers for Henan')


xlim([0 inf])
ylim([0 inf])
%% Bar graph for unique routes
clear
clc
x = categorical({'utexas.edu','google.com','icir.org','nyu.edu','utoronto.ca','gov.za','Moscow','London','Tokyo','Sydney','Henan'});
x = reordercats(x,{'utexas.edu','google.com','icir.org','nyu.edu','utoronto.ca','gov.za','Moscow','London','Tokyo','Sydney','Henan'});
y = [ 139, 1626, 772, 6, 26, 1641, 41, 554, 315, 439, 1524];
b = bar(x,y);
xtips = b.XEndPoints;
ytips = b.YEndPoints;
labels = string(b.YData);
text(xtips,ytips,labels,'HorizontalAlignment','center','VerticalAlignment','bottom')
title('Domains vs # of Unique Routes')
ylabel('# of Unique Routes')
%% Bar graph for hops
clear
clc

Henan = readmatrix('hop-count-210.5.56.145.txt');
Sydney = readmatrix('hop-count-Sydney.txt');
Tokyo = readmatrix('hop-count-Tokyo.txt');
London = readmatrix('hop-count-London.txt');
Moscow = readmatrix('hop-count-Moscow.txt');
Gov = readmatrix('hop-count-Gov.txt');
Toronto = readmatrix('hop-count-utoronto.txt');
NYU = readmatrix('hop-count-nyu.txt');
Icir = readmatrix('hop-count-icir.txt');
Google = readmatrix('hop-count-google.txt');
UTexas = readmatrix('hop-count-utexas.txt');

HenanAvg = ceil(mean(Henan)); 
SydneyAvg = ceil(mean(Sydney));
TokyoAvg = ceil(mean(Tokyo));
LondonAvg = ceil(mean(London));
MoscowAvg = ceil(mean(Moscow));
GovAvg = ceil(mean(Gov));
TorontoAvg = ceil(mean(Toronto));
NYUAvg = ceil(mean(NYU));
IcirAvg = ceil(mean(Icir));
GoogleAvg = ceil(mean(Google));
UTexasAvg = ceil(mean(UTexas));

Hs = std(Henan);
Sstd = std(Sydney);
Tstd = std(Tokyo);
Lstd = std(London);
Mstd = std(Moscow);
GovStd = std(Gov);
TorStd = std(Toronto);
NYUStd = std(NYU);
IcirStd = std(Icir);
GoogStd = std(Google);
UTexasStd = std(UTexas);

error = [UTexasStd, GoogStd, IcirStd, NYUStd, TorStd, GovStd, Mstd, Lstd, Tstd, Sstd, Hs];



y = [UTexasAvg, GoogleAvg, IcirAvg, NYUAvg, TorontoAvg, GovAvg, MoscowAvg, LondonAvg, TokyoAvg, SydneyAvg, HenanAvg];

x = categorical({'utexas.edu','google.com','icir.org','nyu.edu','utoronto.ca','gov.za','Moscow','London','Tokyo','Sydney','Henan'});
x = reordercats(x,{'utexas.edu','google.com','icir.org','nyu.edu','utoronto.ca','gov.za','Moscow','London','Tokyo','Sydney','Henan'});

b = bar(x,y);
hold on 
e = errorbar(y,error,'r');
e.LineStyle = 'none';

xtips = b.XEndPoints;
ytips = b.YEndPoints;
ylim([0 49])
labels = string(b.YData);
text(xtips,ytips,labels,'HorizontalAlignment','center','VerticalAlignment','bottom')
title('Domains vs Average # of Hops')
ylabel('Avg # of Hops')

%% AS Numbers 
x = ['AS32666','AS32666/AS19009','AS600','AS1164','AS15169' ];
y = x

plot(x,y)
%% Geoplot
% lat1 = 41.5066;
% lon1 = -81.611;
% lat2 = 37.751;
% lon2 = -97.822;
% 
% 
% 
% 
% lat3 = 40.7279;
% lon3 = -73.9966;
% geoplot([lat1,lat2,lat3],[lon1,lon2,lon3],'g-*')

lat1 = 41.5066;
lon1 = -81.611;
lat2 = 37.751;
lon2 = -97.822;
lat3 = 33.3815;
lon3 = -117.072;
lat4 = 32.7203;
lon4 = -117.1552;
lat5 = 37.6403;
lon5 = -122.0667;
lat6 = 37.8736;
lon6 = -122.257;
lat7 = 37.7833;
lon7 = -122.1864;
geoplot([lat1,lat2,lat3,lat4,lat5,lat6,lat7],[lon1,lon2,lon3,lon4,lon5,lon6,lon7],'g-*')
%% Actual geo plotter
clear 
clc
A = readmatrix('Tokyo-coords.txt');

A2T = array2table(A,"VariableNames",["Latitude","Longitude"]);

lat = A2T.Latitude;
long = A2T.Longitude;



geoplot(lat,long,'r-*')

geobasemap topographic
title('Path used to get to Tokyo')

