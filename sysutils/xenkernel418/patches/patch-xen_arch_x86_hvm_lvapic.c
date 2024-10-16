$NetBSD: patch-xen_arch_x86_hvm_lvapic.c,v 1.1 2024/09/27 12:45:49 manu Exp $

From https://xenbits.xen.org/xsa/xsa462.patch

From: Jan Beulich <jbeulich@suse.com>
Subject: x86/vLAPIC: prevent undue recursion of vlapic_error()

With the error vector set to an illegal value, the function invoking
vlapic_set_irq() would bring execution back here, with the non-recursive
lock already held. Avoid the call in this case, merely further updating
ESR (if necessary).

This is XSA-462 / CVE-2024-45817.

Fixes: 5f32d186a8b1 ("x86/vlapic: don't silently accept bad vectors")
Reported-by: Federico Serafini <federico.serafini@bugseng.com>
Reported-by: Andrew Cooper <andrew.cooper3@citrix.com>
Signed-off-by: Jan Beulich <jbeulich@suse.com>
Signed-off-by: Andrew Cooper <andrew.cooper3@citrix.com>
Reviewed-by: Andrew Cooper <andrew.cooper3@citrix.com>

diff --git a/xen/arch/x86/hvm/vlapic.c b/xen/arch/x86/hvm/vlapic.c
index 2ec95942713e..8758c4217fab 100644
--- ./xen/arch/x86/hvm/vlapic.c.orig
+++ ./xen/arch/x86/hvm/vlapic.c
@@ -112,9 +112,24 @@ static void vlapic_error(struct vlapic *vlapic, unsigned int errmask)
     if ( (esr & errmask) != errmask )
     {
         uint32_t lvterr = vlapic_get_reg(vlapic, APIC_LVTERR);
+        bool inj = false;
 
-        vlapic_set_reg(vlapic, APIC_ESR, esr | errmask);
         if ( !(lvterr & APIC_LVT_MASKED) )
+        {
+            /*
+             * If LVTERR is unmasked and has an illegal vector, vlapic_set_irq()
+             * will end up back here.  Break the cycle by only injecting LVTERR
+             * if it will succeed, and folding in RECVILL otherwise.
+             */
+            if ( (lvterr & APIC_VECTOR_MASK) >= 16 )
+                 inj = true;
+            else
+                 errmask |= APIC_ESR_RECVILL;
+        }
+
+        vlapic_set_reg(vlapic, APIC_ESR, esr | errmask);
+
+        if ( inj )
             vlapic_set_irq(vlapic, lvterr & APIC_VECTOR_MASK, 0);
     }
     spin_unlock_irqrestore(&vlapic->esr_lock, flags);
