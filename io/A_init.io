
// Create a Proto with the same name as the addon to play nice with AddonLoader.
Protos Addons CInvoke CInvoke := Protos Addons CInvoke
CInvoke do(
	//metadoc CInvoke copyright Trevor Fancher, 2006
	//metadoc CInvoke category binding
	//metadoc CInvoke license New BSD license

	// Because of the way building addons works, our Protos are prefixed
	// with CInvoke.  Here we remove those prefixes.
	method(
		CInvoke slotNames foreach(slot,
			slot beginsWithSeq("CInvoke") and(slot size > 7) ifTrue(
				CInvoke setSlot(slot exclusiveSlice(7), CInvoke getSlot(slot))
				CInvoke removeSlot(slot)
			)
		)
	) call

	removeSlot("type") // we are actually a namespace, not a type
)
