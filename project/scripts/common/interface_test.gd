class_name InterfaceTest extends Object

static func has_interface(node: Node) -> InterfaceTest:
    if "interface_test" in node:
        return node.interface_test
    return null

# override this on node implementation
func imethod():
    pass
