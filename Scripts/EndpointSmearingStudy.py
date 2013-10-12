#!/sw/bin/python2.7

from ucnacore.LinFitter import *
from ucnacore.PyxUtils import *
from ucnacore.QFile import *
import os

def EndpointSmearingDat():
	baseDir = os.environ["UCNA_ANA_PLOTS"]+"/SimSpectrum/EndpointSmear/"
	kFits = QFile(baseDir+"/EndpointSmear_150-XXX.txt").dat["kurieFit"] # <--

	gHeight = 12
	g=graph.graphxy(width=20,height=gHeight,
			   x=graph.axis.lin(title="PMT average energy resolution [keV at 1 MeV]",min=40,max=200),
			   y=graph.axis.lin(title="simulated endpoint shift [keV]"),
			   key = graph.key.key(pos="tl"))
	setTexrunner(g)


	ep0 = 710
	
	
	sens = []
	nn = 0
	pmtRes = [75,100,125]
	resSty = [[style.linestyle.dashed,style.linewidth.THick],
				[style.linestyle.solid,style.linewidth.THick],
				[style.linestyle.dotted,style.linewidth.THick]]
	
	for fitEnd in [600,620,640,660,680,700,720][::-1]:
	#for fitStart in [100,120,140,160,180,200,220,240][::-1]:
	
		lsty = [[style.linestyle.solid],
				[style.linestyle.dashed],
				[style.linestyle.dotted],
				[style.linestyle.dashdotted],
				[style.linestyle.solid,style.linewidth.THick],
				[style.linestyle.dashed,style.linewidth.THick],
				[style.linestyle.dotted,style.linewidth.THick],
				[style.linestyle.dashdotted,style.linewidth.THick]][::-1][nn+1] # <--
		nn+=1
		
		resPts = {}
		for k in kFits:
			k.loadFloats()
			if k.fitEnd != fitEnd: # <--
				continue
			k.endpoint = k.endpoint-ep0
			resPts.setdefault(k.PEperMEV,[]).append(k)
		avgd = [[1000/sqrt(k)]+musigma([c.endpoint for c in resPts[k]])+musigma([c.dendpoint for c in resPts[k]]) for k in resPts]
		avgd.sort()
		if not avgd:
			continue
			
		QF = LinearFitter([polyterm(i) for i in range(3)])
		QF.fit(avgd,cols=(0,1,2),errorbarWeights=False)
		sens.append([fitEnd,] + [nderiv(QF,x) for x in pmtRes])	# <--
		print sens[-1][0],"100keV Res Sensitivity dy/dx =",sens[-1][1]

		g.plot(graph.data.points(avgd,x=1,y=2,dy=3,title=None),[graph.style.errorbar(),graph.style.symbol(symbol.circle,size=0.15)])
		g.plot(graph.data.points(QF.fitcurve(40,200,200),x=1,y=2,title="150--%i keV"%fitEnd),[graph.style.line(lsty)]) # <--



	g.writetofile(baseDir+"Endpoint_End.pdf") # <--

	if 1:
		g=graph.graphxy(width=gHeight,height=gHeight,
				   x=graph.axis.lin(title="Kurie fit window end [keV]"), #min=100,max=240,parter=graph.axis.parter.linear(tickdists=[20,5])), # <--
				   y=graph.axis.lin(title="sensitivity to energy resolution keV/keV"),
				   key = graph.key.key(pos="tl"))
		setTexrunner(g)
		
		for (n,p0) in enumerate(pmtRes):
			QF = LinearFitter([polyterm(i) for i in range(3)])
			QF.fit(sens,cols=(0,n+1))
			g.plot(graph.data.points(sens,x=1,y=n+2,title=None),[graph.style.symbol(symbol.circle)])
			g.plot(graph.data.points(QF.fitcurve(600,720,200),x=1,y=2,title="%i keV @ 1 MeV"%p0),[graph.style.line(resSty[n])]) # <--
			
		g.writetofile(baseDir+"Endpoint_EndSens.pdf") # <--


	if 0:
		g=graph.graphxy(width=15,height=8,
				   x=graph.axis.lin(title="predicted sigma"),
				   y=graph.axis.lin(title="observed sigma"))
		setTexrunner(g)
		g.plot(graph.data.points(avgd,x=4,y=3),[graph.style.symbol(symbol.circle)])
		g.writetofile(baseDir+"EndpointSigma.pdf")

if __name__ == "__main__":
	EndpointSmearingDat()
